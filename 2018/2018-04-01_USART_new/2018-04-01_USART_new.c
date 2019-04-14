
//blue (left) - ground
//green - send to PC (pa9) to (RX) 
//yellow (right) - get from pc (pa10) (TX)
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

#define btn_on()							(GPIOA->IDR & GPIO_IDR_0)

float myTick;
uint16_t ms;
float flagLEDgreen = 0;
float flagLEDblue = 0;

uint8_t message[255];
uint8_t stringtosend[255];
uint8_t send = 0;
uint8_t USART_count = 0;
uint8_t flag_btn_trg = 0;

void USART_clear_message(void);
void USART_send(void);
void USART_EHO(void);
uint8_t USART_GetSTR(void);
void USART_SendSTR(uint8_t *STR);
void USART_SendCharacter(uint8_t data);
void USART_Config(void);
void Delay(void);
void send_to_uart(uint8_t data);
void LEDConfig(void);
void EXTIConfig(void);
void LEDConfig(void);
void BtnTrig(void);



void USART_PutChar(uint8_t ch)
{
  //while(!(USART1->ISR & USART_SR_TC));
  USART1->TDR = ch;
}

void USART_PutString(uint8_t * str)
{
while(*str != 0)
  {
  USART_PutChar(*str);
  str++;
}
}

void EXTI0_1_IRQHandler(void)
{
	BtnTrig();
	EXTI->PR |= EXTI_PR_PR0;		
}


void USART1_IRQHandler(void)
{
	//receive
  SETLEDblue();
	if (USART1 -> ISR & USART_ISR_RXNE)
	{
		if(USART_count>255)
		{
			USART_count = 0;
		}
		message[USART_count++] = USART1->RDR;	
	}	
	RESETLEDblue();
	
	//send
//	if ((USART1->ISR & USART_ISR_TC) == USART_ISR_TC)
	{
//		if (send == sizeof(stringtosend))
//		{
//			send=0;
//			USART1->ICR |= USART_ICR_TCCF; /* Clear transfer complete flag */
//			RESETLEDyellow();
//		}
//		else
//		{
//			SETLEDyellow();
//			/* clear transfer complete flag and fill TDR with a new char */
//			USART1->TDR = stringtosend[send++];
//		}
//		if(send==5)
//		{
//			send = 0;
//		}
//		else
//		{
//			USART1->TDR = stringtosend[send++];
//		}
			
	}

}

void SysTick_Handler()
{
	myTick++;
	ms++;
}


int main()
{
	SysTick_Config(SystemCoreClock/1000);

	//USART1->BRR = (SystemCoreClock / 115200);
	
	LEDConfig();
	EXTIConfig();
	USART_Config();
	USART1->BRR = (SystemCoreClock / 115200);
	
	ms = 0;
	SETLEDgreen();

	
	
	while(1) 
	{
		if(myTick>1000)
		{
			myTick = 0;
			
		   USART_PutString("AB");
		}
	}
}



void BtnTrig(void)
{
	if(flag_btn_trg) {flag_btn_trg = 0; RESETLEDred();}
	else 						 {flag_btn_trg = 1; SETLEDred();}
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

void USART_Config(void)
{
	//blue (left) - ground
	//green - send to PC (pa9)
	//yellow (right) - get from pc (pa10)
	
	RCC -> AHBENR |= RCC_AHBENR_GPIOAEN;
	RCC -> APB2ENR |=RCC_APB2ENR_USART1EN;

	GPIOA -> MODER &= ~GPIO_MODER_MODER9;
	GPIOA -> MODER |= GPIO_MODER_MODER9_1;//alternative mode
	GPIOA -> OSPEEDR |= GPIO_OSPEEDER_OSPEEDR9; //hight - 50MHz
	GPIOA -> OTYPER &= ~GPIO_OTYPER_OT_9;//"0" push-pull
	//GPIOA -> PUPDR |= GPIO_PUPDR_PUPDR9_0;//pull-up "01" NO!
	
	GPIOA->AFR[1] |= 0x00000010; //page170 ref man
	GPIOA->AFR[0] |= 0;
	
	GPIOA -> MODER &= ~GPIO_MODER_MODER10;
	GPIOA -> MODER |= GPIO_MODER_MODER10_1;//alt mode
	GPIOA -> OSPEEDR |= GPIO_OSPEEDER_OSPEEDR10; //higth - 50MHz
	GPIOA -> OTYPER &= ~GPIO_OTYPER_OT_10; //'0' push-pull

	GPIOA->AFR[1] |= 0x00000100; //page170 ref man
	GPIOA->AFR[0] |= 0;
	
	//0x1A0B;//48000000/12;//480000/96;//0x341;//0x1601;//0x9C4;//0x34D;//480000/96;//1388;//0x681; //0x681;//0x9C4;// 0x681; 0x341h 0x64
	
	// (8000000/16/9600) = 52.083; 52->0x34; 0.83*16->0x1; -> 0x341 !!! for 9600 boudrate
	//USART1 -> BRR = 0x341; //9600
	//USART1 -> BRR = 0x8B; //57600
	USART1 -> BRR = 0x4; //115200
	
	
	USART1 -> CR2 |= USART_CR2_ABREN;
	USART1 ->CR1 |= USART_CR1_UE;//usart enable
	USART1 ->CR1 |= USART_CR1_TE;//transmitter enable	
	USART1 ->CR1 |= USART_CR1_RE;//receiver enable

//	USART1 ->CR1 &= ~USART_CR1_PCE;//parity control disable
//	USART1 ->CR1 &= ~USART_CR1_M1;//wordlength
//	USART1 ->CR1 &= ~USART_CR1_M0;
//	USART1 ->CR2 &= ~USART_CR2_STOP;//stop bit,00- 1bit
//	USART1 ->CR3 &= ~USART_CR3_RTSE;
//	USART1 ->CR3 &= ~USART_CR3_CTSE;
	
	
	USART1 -> CR1 |= USART_CR1_RXNEIE; // interrupt enable
	NVIC_EnableIRQ(USART1_IRQn);	
	
	
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


