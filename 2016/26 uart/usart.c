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

void USART_config(void);
void Delay(void);
void send_to_uart(uint8_t data);
void MyLEDConfigurationRegisters(void);

void SysTick_Handler()
{
	myTick++;
}

int main()
{
	SysTick_Config(SystemCoreClock/1000);
	
	MyLEDConfigurationRegisters();
	USART_config();
	
	
	
	while(1) 
	{
		if(myTick>10)
		{
			myTick = 0;
			while(!(USART1->ISR & USART_ISR_TC));//wait for transmission complete
			USART1->TDR= 0x00;			
		}
	}
}



void USART_config(void)
{
	RCC -> AHBENR |= RCC_AHBENR_GPIOAEN;
	RCC -> APB2ENR |=RCC_APB2ENR_USART1EN;
	
	GPIOA -> MODER &= ~GPIO_MODER_MODER9;
	GPIOA -> MODER |= GPIO_MODER_MODER9_1;//alternative mode
	GPIOA -> OSPEEDR |= GPIO_OSPEEDER_OSPEEDR9; //hight - 50MHz
	GPIOA -> OTYPER &= ~GPIO_OTYPER_OT_9;//"0" push-pull
	//GPIOA -> PUPDR |= GPIO_PUPDR_PUPDR9_0;//pull-up "01" NO!
	GPIOA->AFR[1] |= 0x00000010;
	GPIOA->AFR[0] |= 0;
	
	//0x341 время на 9 бит - 920мкс - норм
	USART1 -> BRR = 0x341;//0x1A0B;//48000000/12;//480000/96;//0x341;//0x1601;//0x9C4;//0x34D;//480000/96;//1388;//0x681; //0x681;//0x9C4;// 0x681; 0x341h 0x64
	USART1 -> CR2 |= USART_CR2_ABREN;
	USART1 ->CR1 |= USART_CR1_UE;//usart enable
	USART1 ->CR1 |= USART_CR1_TE;//transmitter enable	
	USART1 ->CR1 |= USART_CR1_RE;

//	USART1 ->CR1 &= ~USART_CR1_PCE;//parity control disable
//	USART1 ->CR1 &= ~USART_CR1_M1;//wordlength
//	USART1 ->CR1 &= ~USART_CR1_M0;
//	USART1 ->CR2 &= ~USART_CR2_STOP;//stop bit,00- 1bit
//	USART1 ->CR3 &= ~USART_CR3_RTSE;
//	USART1 ->CR3 &= ~USART_CR3_CTSE;
	
	

}

void MyLEDConfigurationRegisters(void)
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
}//END OF void MyLEDConfigurationRegisters();




