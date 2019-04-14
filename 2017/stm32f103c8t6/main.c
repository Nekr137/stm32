#include "stm32f10x.h"                  // Device header

uint32_t msTick;


void SysTick_Handler(void)
{
	msTick++;
	
}

int main()
{
	uint32_t PA0flag; 
	
	SysTick_Config(SystemCoreClock/1000);
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	
	GPIOC->CRH |= GPIO_CRH_MODE13_0;
	GPIOC->CRH &= ~GPIO_CRH_CNF13;
	PA0flag = 0;
	
	while(1)
	{
		if(msTick>100)
		{
			msTick = 0;
			if(PA0flag == 0)
			{
				PA0flag = 1;
				//GPIOA->BSRR |= GPIO_BSRR_BS_0;
				GPIOC->BSRR |= GPIO_BSRR_BS13;
			}
			else
			{
				PA0flag = 0;
				//GPIOA->BSRR |= GPIO_BSRR_BR_0;
				GPIOC->BSRR |= GPIO_BSRR_BR13;
			}
		}
		
	}
	
}

