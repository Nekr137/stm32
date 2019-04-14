//questions
//Иногда начинает писать кракозябры. Заметил, что когда не все буквы приняты компом, а комп в это время
//что-то отправляет. Поэтому, например, чтобы писать вертикально, нужно париться. 
//Видимо, проблема в get, нужно чтобы он сразу всё слово забирал в переменную
//Интересно, что когда вначале пошлешь 1, то после все работает, а если сразу забабахать большое 
//слово - кракозабры. 
//Не, все-таки проблема в терминале. Может, научиться и скорость поменьше сделать, проверок побольше. 

	//blue (left) - ground
	//green - send to PC (pa9) (RX)
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

float myTick;
float flagLEDgreen = 0;
float flagLEDblue = 0;

uint8_t message[255];
uint8_t USART_count = 0;

void USART_clear_message(void);
void USART_send(void);
void USART_EHO(void);
uint8_t USART_GetSTR(void);
void USART_SendSTR(uint8_t *STR);
void USART_SendCharacter(uint8_t data);
void USART_Config(void);
void Delay(void);
void send_to_uart(uint8_t data);
void MyLED_Config(void);


void USART1_IRQHandler(void)
{
	if (USART1 -> ISR & USART_ISR_RXNE)
	{
		message[USART_count++] = USART1->RDR;	
	}
}

void SysTick_Handler()
{
	myTick++;
}


int main()
{
	SysTick_Config(SystemCoreClock/1000);

	MyLED_Config();
	USART_Config();
	
	
	USART_SendSTR("Start");
	
	while(1) 
	{
		if(myTick>1000)
		{
			myTick = 0;
			
			USART_SendSTR("...\n");
			USART_SendSTR(message);
			if(*message)
			{
				USART_SendSTR("\n");
				USART_clear_message();
			}
		}
	}
}

void USART_clear_message(void)
{
	for(uint8_t t=0; t<10; t++) {message[t]=0;}
	USART_count = 0;
}

void USART_SendSTR(uint8_t *STR)
{
    uint8_t a;   
    while (1) 
		{
        a = *STR;
        if(a) 
				{
            USART_SendCharacter(a);
        }
        else break;      
        STR++;//move to the next     
    } 
}

void USART_SendCharacter(uint8_t data)
{
	while(!(USART1->ISR & USART_ISR_TC));//wait for transmission complete
	USART1->TDR= data;
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
	USART1 -> BRR = 0x4; 
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


void MyLED_Config(void)
{
	RCC -> AHBENR |= RCC_AHBENR_GPIOCEN;
	//GPIO port mode register
			GPIOC->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7 | GPIO_MODER_MODER8 | GPIO_MODER_MODER9);
			GPIOC->MODER |= (GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0);
	
	//GPIO port output type register (GPIOx_OTYPER)
			GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7 | GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9);
					
	//GPIO port output speed register (GPIOx_OSPEEDR) (x = A..F)
			GPIOC->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7 | GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9);
					
	//GPIO port pull-up/pull-down register (GPIOx_PUPDR) (x = A..F)
			GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR7 | GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9);
}


//	USART_SendCharacter(0xA);//enter


