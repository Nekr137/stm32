#include "stm32f0xx.h"                  // Device header

uint32_t var= 0;
uint8_t flag_plus = 1;

void PWM_init(void);


int main()
{
	PWM_init();
	while(1)
	{
		if(TIM3 ->SR & TIM_SR_UIF)
		{
			TIM3 ->SR |= ~TIM_SR_UIF;
			
			if(var > 5000)	flag_plus = 0; 
			if(var <=0)	flag_plus = 1;			
			if(flag_plus==1) var = var+1; else var = var-1;
			
			TIM3->CCR1 = var/10;
			TIM3->CCR2 = var/10;
			TIM3->CCR3 = var/10;
			TIM3->CCR4 = var/10;
		}
	}
	
}



void PWM_init(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	
	GPIOC -> MODER &= ~(GPIO_MODER_MODER6|GPIO_MODER_MODER7|GPIO_MODER_MODER8|GPIO_MODER_MODER9);
	GPIOC -> MODER |= (GPIO_MODER_MODER6_1|GPIO_MODER_MODER7_1|GPIO_MODER_MODER8_1|GPIO_MODER_MODER9_1);//alternative mode
	GPIOC -> OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR6|GPIO_OSPEEDER_OSPEEDR7|GPIO_OSPEEDER_OSPEEDR8|GPIO_OSPEEDER_OSPEEDR9);
	GPIOC -> OTYPER &= ~(GPIO_OTYPER_OT_6|GPIO_OTYPER_OT_7|GPIO_OTYPER_OT_8|GPIO_OTYPER_OT_9);
	GPIOC->AFR[0] &= ~(GPIO_AFRL_AFRL6);
	
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;  //enable 
	
	
	TIM3->CR1 = 0; //when timer is disabled
	TIM3->CR2 = 0; 
  TIM3->DIER = 0;
	TIM3->CCMR1 = (TIM_CCMR1_OC1M_1|TIM_CCMR1_OC1M_2|TIM_CCMR1_OC2M_1|TIM_CCMR1_OC2M_2);// other bits = 0
	TIM3->CCMR2 = (TIM_CCMR2_OC3M_1|TIM_CCMR2_OC3M_2|TIM_CCMR2_OC4M_1|TIM_CCMR2_OC4M_2);
	TIM3->PSC = 80-1; //f-1 - devided by 8e3 (because of prescaler, because of zero division)
	TIM3->ARR = 1000;
	

	TIM3->CCR1 = 40; TIM3->CCR2 = 40;TIM3->CCR3 = 40;TIM3->CCR4 = 40;
	TIM3->CCER = (TIM_CCER_CC1E|TIM_CCER_CC2E|TIM_CCER_CC3E|TIM_CCER_CC4E);
	TIM3->EGR = TIM_EGR_UG;
	TIM3->CR1 |= TIM_CR1_CEN;

	
}
