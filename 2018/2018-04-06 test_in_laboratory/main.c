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

volatile uint16_t msTick;
uint8_t flag = 0;

void LEDConfig(void);
void BTN_PA0Config(void);

void SysTick_Handler()
{
	msTick++;
}

int main(void)
{
	SysTick_Config(SystemCoreClock/1000);
	volatile uint32_t S = SystemCoreClock;
	LEDConfig();
	
	while(1)
	{
	
		if(msTick>10)
		{
			msTick  = 0;
			switch(flag)
			{
				case 0:
					SETLEDblue();
					flag = 1;
					break;
				case 1:
					RESETLEDblue();
					flag = 0;
					break;
			}
		}
	}
}

void BTN_PA0Config(void)
{
		RCC->AHBENR |= RCC_AHBENR_GPIOAEN; 		/* (1) Enable the peripheral clock of GPIOA */
		SYSCFG->EXTICR[1] &= (uint16_t)~SYSCFG_EXTICR1_EXTI0_PA; /* (2) Select Port A for pin 0 external interrupt by writing 0000 in EXTI0 (reset value)*/ 
		GPIOA->MODER &= ~GPIO_MODER_MODER0; /* input mode */
		GPIOA->PUPDR |= ~GPIO_PUPDR_PUPDR0; /* no pull-up no pull-down (floating input) */
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
