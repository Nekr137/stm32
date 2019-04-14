
#include "MyADC.h"



void ADCconfig(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	RCC->AHBENR |=RCC_AHBENR_GPIOCEN;
	RCC->AHBENR |=RCC_AHBENR_GPIOAEN;

	GPIOA->MODER &= ~GPIO_MODER_MODER1;		//reset pin4 
	GPIOA->MODER |= GPIO_MODER_MODER1;		//analog
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR1_1;	//pull-down
	
	ADCcalib();
	
	ADC1->CR |= ADC_CR_ADEN;							//adc1 enable
	while(!(ADC1->ISR & ADC_ISR_ADRDY))
	{
	}
	ADC1->CFGR1 |= ADC_CFGR1_CONT;				//contineously
	ADC1->CHSELR = ADC_CHSELR_CHSEL1;			//chnnel 1
	ADC1->CR |= ADC_CR_ADSTART;						//start
	
}

uint32_t ADCread(void)
{
	uint32_t data = 0;
	if(ADC1->ISR & ADC_ISR_EOC)						//if == 1;
	{
		data = ADC1->DR;										//read from register
	}
	return data;
}

void ADCcalib(void)											//taken from reference manual
{
	/* (1) Ensure that ADEN = 0 */
	/* (2) Clear ADEN */
	/* (3) Launch the calibration by setting ADCAL */
	/* (4) Wait until ADCAL=0 */
	if ((ADC1->CR & ADC_CR_ADEN) != 0) /* (1) */
	{
		ADC1->CR &= (uint32_t)(~ADC_CR_ADEN); /* (2) */
	}
	ADC1->CR |= ADC_CR_ADCAL; /* (3) */
	while ((ADC1->CR & ADC_CR_ADCAL) != 0) /* (4) */
	{
		/* For robust implementation, add here time-out management */
	}	
}

