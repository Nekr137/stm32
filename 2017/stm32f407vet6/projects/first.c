
#include "stm32f4xx.h"                  // Device header

uint32_t msTick;


void PA0_config(void);

void SysTick_Handler(void)
{
	msTick++;
	
}

int main()
{
	uint32_t PA0flag; 
	
	SysTick_Config(SystemCoreClock/1000);
	
	PA0_config();
	PA0flag = 0;
	
	while(1)
	{
		if(msTick>1000)
		{
			msTick = 0;
			if(PA0flag == 0)
			{
				PA0flag = 1;
				GPIOA->BSRR |= GPIO_BSRR_BS_0;
			}
			else
			{
				PA0flag = 0;
				GPIOA->BSRR |= GPIO_BSRR_BR_0;
			}
		}
		
	}
	
}

void PA0_config(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	
	GPIOA->MODER &= ~GPIO_MODER_MODER0;	
	GPIOA->MODER |= GPIO_MODER_MODER0_0;
}	


