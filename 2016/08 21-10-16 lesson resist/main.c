//lesson 21.10.16 RESIST


#include "stm32f0xx.h"                  // Device header

#define PortAClocking()				(RCC->AHBENR |= RCC_AHBENR_GPIOAEN)
#define PortCClocking()				(RCC->AHBENR |= RCC_AHBENR_GPIOCEN)

#define ADCClocking()					(RCC->APB2ENR |= RCC_APB2ENR_ADCEN)

//MY LED macros
#define SETLEDred()						(GPIOC->BSRR = GPIO_BSRR_BS_6)
#define SETLEDgreen()					(GPIOC->BSRR = GPIO_BSRR_BS_9)
#define SETLEDblue()					(GPIOC->BSRR = GPIO_BSRR_BS_7)
#define SETLEDyellow()				(GPIOC->BSRR = GPIO_BSRR_BS_8)
#define RESETLEDred()					(GPIOC->BSRR = GPIO_BSRR_BR_6)
#define RESETLEDgreen()				(GPIOC->BSRR = GPIO_BSRR_BR_9)
#define RESETLEDblue()				(GPIOC->BSRR = GPIO_BSRR_BR_7)
#define RESETLEDyellow()			(GPIOC->BSRR = GPIO_BSRR_BR_8)

#define BUTTON_IN()						(GPIOA->IDR & GPIO_IDR_0)

#define On 1;
#define Off 0;
#define Up 1;
#define Down 0;


int msCount = 0;

void MyClocking(void);
void MyLEDConfigurationRegisters(void);


void SysTick_Handler()
{
	msCount++;
}



int main(void)
{
	volatile int data;
	SysTick_Config(SystemCoreClock/1000);// 1000HZ TICK
	MyClocking();
	
	
	MyLEDConfigurationRegisters();
	
	GPIOA->MODER |= GPIO_MODER_MODER1;
	
	
	ADCClocking();
	
	ADC1->CR = ADC_CR_ADCAL;//"=" - all disabled 
	while((ADC1->CR & ADC_CR_ADCAL)!=0)
	{
	}

	ADC1->CR |= ADC_CR_ADEN; //adc begin work contiieosly!
	while(!(ADC1->ISR & ADC_ISR_ADRDY))
	{
	}
	
	ADC1->CFGR1 |= ADC_CFGR1_CONT;
	//config1 
	ADC1->CHSELR = ADC_CHSELR_CHSEL1;//channel 1
	
	ADC1->CR |= ADC_CR_ADSTART;
	
	

	
	while(1)
	{
		if(ADC1->ISR & ADC_ISR_EOC)
		{
			data = ADC1->DR;//reed from register
			//ADC1->ISR |= ADC_ISR_EOC;//write "1" (interesting moment)
		}
		RESETLEDgreen();
		RESETLEDred();
		RESETLEDyellow();
		
		
		
		if(data<500)
		{
			
		}
		if((data>500) && (data<1500))
		{
			SETLEDyellow();
		}
		if((data>1500) && (data<2500))
		{
			SETLEDred();
			SETLEDyellow();
		}
		if((data>2500) && (data<4096))
		{
			SETLEDred();
			SETLEDyellow();
			SETLEDgreen();			
		}

	}
	
}


void MyClocking(void)
{
		//RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
		PortAClocking();
	  //RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
		PortCClocking();
	
		
}


void MyLEDConfigurationRegisters(void)
{
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
