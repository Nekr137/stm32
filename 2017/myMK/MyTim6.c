#include "MyTim6.h"


void Tim6config(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
	
	TIM6->PSC = 0x0001;												//prescaler register
	TIM6->ARR = 0x0001;												//auto reload register (period) 0x0025 - 100kHz
	TIM6->DIER |= TIM_DIER_UIE;									//update interrupt enable
	TIM6->CR1 |= TIM_CR1_CEN;										//counter enable
}



////обработчик прерывания от таймера
//void TIM6_IRQHandler(void)
//{
//	TIM6->SR &= ~TIM_SR_UIF;										//clear flag!!
//	if (i == 0) GPIOB->BSRRL |= GPIO_BSRR_BS_6;
//  else GPIOB->BSRRH |= GPIO_BSRR_BS_6;
//  i = ~i;
//}

