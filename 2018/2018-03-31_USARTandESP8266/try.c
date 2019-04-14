
//PC10 TRIGGER CH_PD
//PA9 - SEND to (RX in FT232RL) 
//PA2 USART2 SEND 
//PA10 - RECEIVE (TX in FT232RL)
//PA3 USART2 RECEIVE
//power - automatically

#include "stm32f0xx.h"

#define SETLEDred()						(GPIOC->BSRR = GPIO_BSRR_BS_6)
#define SETLEDgreen()					(GPIOC->BSRR = GPIO_BSRR_BS_9)
#define SETLEDblue()					(GPIOC->BSRR = GPIO_BSRR_BS_7)
#define SETLEDyellow()				(GPIOC->BSRR = GPIO_BSRR_BS_8)

#define RESETLEDred()					(GPIOC->BSRR = GPIO_BSRR_BR_6)
#define RESETLEDgreen()				(GPIOC->BSRR = GPIO_BSRR_BR_9)
#define RESETLEDblue()				(GPIOC->BSRR = GPIO_BSRR_BR_7)
#define RESETLEDyellow()			(GPIOC->BSRR = GPIO_BSRR_BR_8)

#define CH_PD_ON()						(GPIOC->BSRR = GPIO_BSRR_BS_12)
#define CH_PD_OFF()						(GPIOC->BSRR = GPIO_BSRR_BR_12)

#define btn_on()							(GPIOA->IDR & GPIO_IDR_0)

uint16_t msTick;
uint16_t ms;
float flagLEDgreen = 0;
float flagLEDblue = 0;
volatile int temperature;

uint8_t message1[255];
uint8_t message4[255];
uint8_t message4_k1 = 0;
uint8_t message4_k2 = 0;

uint8_t USART1_count = 0;
uint8_t USART4_count = 0;
uint8_t flag_btn_trg = 0;
uint16_t datatosend = 0;
char *request_string = "";


void clear_message4(void);
uint8_t ESP8266_SendATandWaitForAnswer(uint8_t *AT,char ** answer);
uint8_t StringExist(uint8_t *str,char *str_in);
void clear_array(uint8_t *array,uint8_t len);
void ESP8266init(void);
void Pause(uint16_t t);
void USART_clear_message(void);
void USART_SendChar(char *STR,uint8_t USARTnumber);
void USART_send(void);
void USART_EHO(void);
uint8_t USART_GetSTR(void);
void USART_SendSTR(uint8_t *STR, uint16_t time, uint8_t USARTnumber);
void USART_SendINT(uint8_t value, uint8_t USARTnumber);
void USART_SendCharacter(uint8_t data,uint8_t USARTnumber);
void ESP8266_SendATandWaitForAnswerRequest(char*address, char*var,int value, uint8_t USARTnumber);
void USART1_Config(void);
void USART4_Config(void);
void Delay(void);
void send_to_uart(uint8_t data);
void LEDConfig(void);
void EXTIConfig(void);
void LEDConfig(void);


void EXTI0_1_IRQHandler(void)
{
	if(flag_btn_trg) {flag_btn_trg = 0; RESETLEDred();}
	else 						 {flag_btn_trg = 1; SETLEDred();}
	EXTI->PR |= EXTI_PR_PR0;					
}

void USART1_IRQHandler(void)
{	
	if (USART1 -> ISR & USART_ISR_RXNE)
	{
		SETLEDgreen();
		//USART1 -> ISR &= ~USART_ISR_RXNE;
		message1[USART1_count++] = USART1->RDR;	
		RESETLEDgreen();
	}	
}

void USART3_4_IRQHandler(void)
{
	if (USART4 -> ISR & USART_ISR_RXNE)
	{
		SETLEDblue();
		message4[message4_k2++] = USART4->RDR;
		USART_SendCharacter(message4[message4_k2-1],1);
		if(message4_k2>255)
		{
			message4_k2 = 0;
		}
		RESETLEDblue();
	}
}

void SysTick_Handler()
{
	msTick++;
	ms++;
}


int main()
{
	SysTick_Config(SystemCoreClock/1000);

		
	LEDConfig();
	EXTIConfig();
	USART1_Config();
	USART4_Config();
	
	USART1->BRR = (SystemCoreClock / 115200);	
	USART4->BRR = (SystemCoreClock / 115200);
	
	ESP8266init();

			
	while(1) 
	{
		if(msTick>1000)
		{
			SETLEDgreen();
			msTick = 0;
			temperature += 1;

			ESP8266_SendATandWaitForAnswerRequest("GET /index.php","t",10,4);
			USART_SendSTR("Host: 192.168.0.95\r\n",0,4);
			USART_SendSTR("Connection: close\r\n",0,4);
			RESETLEDgreen();
			SETLEDblue();
			USART_SendSTR("\r\n",0,4);
			USART_SendSTR(message4,0,1);
			clear_message4();
		
			
			Pause(1000);
//			clear_message4();
			
		}
	}
	
}


void ESP8266_SendATandWaitForAnswerRequest(char*address, char*var,int value, uint8_t USARTnumber)
{
	//USART_SendSTR("GET /index.php?t=6 HTTP/1.1\r\n",0);	
	  uint8_t a;   
    while (1) 
		{
        a = *address;
        if(a) 
				{
					USART_SendCharacter(a,USARTnumber);
        }
        else break;      
        address++;//move to the next     
    } 
		USART_SendSTR("?",0,USARTnumber);
		while (1) 
		{
        a = *var;
        if(a) 
				{
					USART_SendCharacter(a,USARTnumber);
        }
        else break;      
        var++;//move to the next     
    } 
		USART_SendSTR("=",0,USARTnumber);
		USART_SendINT(value,USARTnumber);
		USART_SendSTR(" HTTP/1.1\r\n",0,USARTnumber);		
}

void USART_SendINT(uint8_t value, uint8_t USARTnumber)
{
			uint8_t tmp[256];		uint16_t k = 0;
		while(value!=0)
		{
			tmp[k] = value%10+'0';
			value /= 10;
			k++;
		}
		
		uint16_t k2;		uint8_t tmp2[256];
		for(k2 = 0; k2<k; k2++)
		{
			tmp2[k2] = tmp[k-k2-1];
		}
		USART_SendSTR(tmp2,0,USARTnumber);
}

void Pause(uint16_t t)
{
	while(ms<t){} ms = 0;
}

void USART_SendSTR(uint8_t *STR,uint16_t time,uint8_t USARTnumber)
{
    uint8_t a;   
    while (1) 
		{
        a = *STR;
        if(a) 
				{
					USART_SendCharacter(a,USARTnumber);
        }
        else break;      
        STR++;//move to the next     
    } 
		Pause(time);
}

void USART_SendChar(char *STR,uint8_t USARTnumber)
{
	    uint8_t a;   
    while (1) 
		{
        a = *STR;
        if(a) 
				{
					USART_SendCharacter(a,USARTnumber);
        }
        else break;      
        STR++;//move to the next     
    } 
}

void USART_SendCharacter(uint8_t data,uint8_t USARTnumber)
{
	if(USARTnumber == 1)
	{
		while(!(USART1->ISR & USART_ISR_TC));//wait for transmission complete
		USART1->TDR= data;
	}
	if(USARTnumber ==4)
	{
		while(!(USART4->ISR & USART_ISR_TC));
		USART4->TDR= data;
	}
}

void EXTIConfig(void)
{
		RCC->AHBENR |= RCC_AHBENR_GPIOAEN; 		/* (1) Enable the peripheral clock of GPIOA */
		SYSCFG->EXTICR[1] &= (uint16_t)~SYSCFG_EXTICR1_EXTI0_PA; /* (2) Select Port A for pin 0 external interrupt by writing 0000 in EXTI0 (reset value)*/ 
		GPIOA->MODER &= ~GPIO_MODER_MODER0; /* input mode */
		GPIOA->PUPDR |= ~GPIO_PUPDR_PUPDR0; /* no pull-up no pull-down (floating input) */ 
	
  	EXTI->IMR = EXTI_IMR_IM0;       /* (3) Configure the corresponding mask bit in the EXTI_IMR register */
		EXTI->RTSR = EXTI_RTSR_RT0;			/* (4) Configure the Trigger Selection bits of the Interrupt line on rising edge*/
		EXTI->FTSR = EXTI_FTSR_FT0; 		/* (5) Interrupt line on falling edge (push-up the button)*/
		
		/* Configure NVIC for External Interrupt */
		NVIC_EnableIRQ(EXTI0_1_IRQn); 				/* (1) Enable Interrupt on EXTI0_1 */
		NVIC_SetPriority(EXTI0_1_IRQn,0); 		/* (2)	 Set priority for EXTI0_1 */
}

void USART4_Config(void)
{
	  // PC10 SEND, PC11 RECEIVE
		RCC -> AHBENR |= RCC_AHBENR_GPIOCEN;
		RCC -> APB1ENR |= RCC_APB1ENR_USART4EN;
	
		GPIOC -> MODER &= ~(GPIO_MODER_MODER10|GPIO_MODER_MODER11);
		GPIOC -> MODER |= (GPIO_MODER_MODER10_1|GPIO_MODER_MODER11_1);//alternative mode
		GPIOC -> OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR10|GPIO_OSPEEDER_OSPEEDR1); //hight - 50MHz
		GPIOC -> OTYPER &= ~(GPIO_OTYPER_OT_10|GPIO_OTYPER_OT_1);//"0" push-pull

		GPIOC->AFR[1] |= 0x00000000; //page170 ref man , Alternative function number 1
		GPIOC->AFR[0] |= 0x00000000;
		USART4 -> BRR = 0x4; //115200
	
	
		//auto baudrate enable
	  //USART4 ->CR2 |= USART_CR2_ABREN;
	  // usart enable | transmitter enable | receiver enable | interrupt enable
	  USART4 ->CR1 |= (USART_CR1_UE|USART_CR1_TE|USART_CR1_RE|USART_CR1_RXNEIE);

		NVIC_EnableIRQ(USART3_4_IRQn);

}

void USART1_Config(void)
{
	//PA9 SEND, PA10 RECEIVE
	RCC -> AHBENR |= RCC_AHBENR_GPIOAEN;
	RCC -> APB2ENR |=RCC_APB2ENR_USART1EN;

	GPIOA -> MODER &= ~(GPIO_MODER_MODER9|GPIO_MODER_MODER10);
	GPIOA -> MODER |= (GPIO_MODER_MODER9_1|GPIO_MODER_MODER10_1);//alternative mode
	GPIOA -> OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR9|GPIO_OSPEEDER_OSPEEDR10); //hight - 50MHz
	GPIOA -> OTYPER &= ~(GPIO_OTYPER_OT_9|GPIO_OTYPER_OT_10);//"0" push-pull
	//GPIOA -> PUPDR |= GPIO_PUPDR_PUPDR9_0;//pull-up "01" NO!
	
	GPIOA->AFR[1] |= 0x00000110; //page170 ref man
	GPIOA->AFR[0] |= 0;


	// (8000000/16/9600) = 52.083; 52->0x34; 0.83*16->0x1; -> 0x341 !!! for 9600 boudrate
	//USART1 -> BRR = 0x341; //9600
	//USART1 -> BRR = 0x8B; //57600
	USART1 -> BRR = 0x4; //115200
	
	//auto baudrate enable
	//USART1 -> CR2 |= USART_CR2_ABREN;
	
	// usart enable, transmitter enable, receive enable
	USART1 ->CR1 |= (USART_CR1_UE|USART_CR1_TE|USART_CR1_RE);
	
//	USART1 ->CR1 &= ~USART_CR1_PCE;//parity control disable
//	USART1 ->CR1 &= ~USART_CR1_M1;//wordlength
//	USART1 ->CR1 &= ~USART_CR1_M0;
//	USART1 ->CR2 &= ~USART_CR2_STOP;//stop bit,00- 1bit
//	USART1 ->CR3 &= ~USART_CR3_RTSE;
//	USART1 ->CR3 &= ~USART_CR3_CTSE;
	
	USART1 -> CR1 |= USART_CR1_RXNEIE; // interrupt enable
	NVIC_EnableIRQ(USART1_IRQn);	
	
}

void ESP8266init(void)
{
	SETLEDyellow();
	
	// TRIGGER CH_PD
	GPIOC->MODER &= ~(GPIO_MODER_MODER12);
	GPIOC->MODER |= (GPIO_MODER_MODER12_0);
	GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_12);
	GPIOC->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR12);
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR12);
	
	// wait 74880 baudrate ending
	USART4 ->CR1 &= ~USART_CR1_RE;
	CH_PD_OFF();	CH_PD_ON(); 	Pause(1000);
	USART_SendSTR("AT+RESTORE\r\n",2000,4);
	USART4 -> CR1 |= USART_CR1_RE;
	
	
	char* p_ans[] = {"ERROR","TS","OK","CONNECTED"};
	char** pp_ans = p_ans;

	SETLEDblue();
	
  uint8_t kkkk = 0;
	kkkk = ESP8266_SendATandWaitForAnswer("AT\r\n",pp_ans);
	USART_SendCharacter('\n',1);
	USART_SendINT(kkkk,1);
	kkkk = ESP8266_SendATandWaitForAnswer("AT+CWMODE=3\r\n",pp_ans);
	USART_SendINT(kkkk,1);
  ESP8266_SendATandWaitForAnswer("AT+CWJAP=\"Andryousha\",\"A19721991a\"\r\n",pp_ans);
  ESP8266_SendATandWaitForAnswer("AT+CIPSTART=\"TCP\",\"10.1.30.36\",80\r\n",pp_ans);
	ESP8266_SendATandWaitForAnswer("AT+CIPMODE=1\r\n",pp_ans);
	ESP8266_SendATandWaitForAnswer("AT+CIPSEND\r\n",pp_ans);
	


//			SETLEDred();
//			USART_SendSTR("AT+CWMODE=3\r\n",1000);
//			//USART_SendSTR("AT+RST\r\n",4000);
//			//USART_SendSTR("AT+CWJAP=\"n\",\"hahahaha\"\r\n");
//			USART_SendSTR("AT+CWJAP=\"X Y i 319b\",\"yanazhopa\"\r\n",6000);
//			//USART_SendSTR("AT+CIPSTART=\"TCP\",\"192.168.0.95\",80\r\n");
//			USART_SendSTR("AT+CIPSTART=\"TCP\",\"192.168.1.183\",80\r\n",8000);
//			USART_SendSTR("AT+CIPMODE=1\r\n",1000);
//			USART_SendSTR("AT+CIPSEND\r\n",3000);
//				
	RESETLEDyellow();
	
}


uint8_t ESP8266_SendATandWaitForAnswer(uint8_t *AT,char ** answer)
{
	uint16_t words = 0;
	uint16_t ans_num = 0; 

	while(answer[words]!='\0')
	{
		words++;
		//Pause(1000);
	}
	words-=2;
	
	
	//wait for "answer" appear in message4
	clear_message4();
	USART_SendSTR(AT,0,4);
	
	uint8_t msTick_ = msTick;
	uint8_t tmp = 0;
		
	while(1)
	{
		if(msTick-msTick_>15000)
		{
			clear_message4();
			USART_SendSTR(AT,0,4); 
			msTick = 0;
			USART_SendSTR("CLEAR MESSAGE 4",0,1);
		}
		for(ans_num = 0; ans_num < words; ans_num++)
		{		
			Pause(500);
			if(StringExist(message4,answer[ans_num]))
			{
				SETLEDgreen();
				RESETLEDred();
				return ans_num;
			}	
		}
		SETLEDred();
	}
}

void clear_message4(void)
{
		message4[message4_k2] = 0;
		while(message4_k2>0)
		{
			message4[message4_k2] = 0;
			message4_k2--;			
		}
}
uint8_t StringExist(uint8_t *str,char *str_in)
{
	uint8_t k1 = 0;	
	uint8_t k2 = 0;
	uint8_t tmp1 = 0;
	uint8_t tmp2 = 'A';
	uint8_t flag_coincide = 0;
	uint8_t str_len = 0;
	uint8_t str_in_len = 0;
	
	while(str[str_len])
	{
		str_len++;
	}
	//while(str[str_len++])
	
	while(str_in[str_in_len])
	{
		str_in_len++;
	}
	
	if(str_len >= str_in_len)
	{
		while(k1<=str_len-str_in_len)
		{
			while(k2<str_in_len)
			{
				tmp1 = str[k1+k2];
				tmp2 = str_in[k2];
				if(tmp1!=tmp2)
				{
					flag_coincide = 0;
					break;
				}
				else
				{
					if(k2 == str_in_len-1)
					{
						if(flag_coincide == 1)
						{
							return 1;
						}
					}
					else
					{
						flag_coincide = 1;
					}
				}
				k2++;
			}
			k2 = 0;		k1++;
		}
	}
	return 0;
}




void LEDConfig(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	GPIOC->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7 | GPIO_MODER_MODER8 | GPIO_MODER_MODER9);
	GPIOC->MODER |= (GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0);
	GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7 | GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9);
	GPIOC->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7 | GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9);
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR7 | GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9);
}


