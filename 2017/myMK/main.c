
#include "MyADC.h"
#include "MyDAC.h"
#include "MyLCD.h"
#include "MyLED.h"
#include "MyGyr.h"
#include "MyOsc.h"
#include "MyButton.h"
#include "MyNVIC.h"
#include "MySPI.h"
#include "MyTim6.h"

uint16_t msTick;

void TIM6_IRQHandler(void);

void SysTick_Handler()
{
	msTick++;
}

int main()
{
	uint32_t indx = 0;

	SysTick_Config(SystemCoreClock/1000);

	LEDconfig();

	
	while(1)
	{
		SETLEDgreen();
	}
}

void TIM6_IRQHandler(void)
{
//	TIM6->SR &= ~TIM_SR_UIF;										//clear flag!!
//	if (i == 0) GPIOB->BSRRL |= GPIO_BSRR_BS_6;
//  else GPIOB->BSRRH |= GPIO_BSRR_BS_6;
//  i = ~i;
}




