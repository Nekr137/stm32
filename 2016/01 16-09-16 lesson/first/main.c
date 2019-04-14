#include "stm32f0xx.h" 

void sos(int t);

int main(void)
{

		static uint32_t i;
	
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN; //vkluqili

	
	//out pin
	GPIOC->MODER &= ~GPIO_MODER_MODER6; //sbrosili
	GPIOC->MODER |= GPIO_MODER_MODER6_0;//mladshiy bit vistavili na 1
	
	//registro-type
	GPIOC->OTYPER &= ~GPIO_OTYPER_OT_6; //zapisali nolik
	
	//registr ospeedr
	GPIOC->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR6;
	
	//registr pupd
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR6;
	
	
	
	
//	while(1)
//	{
//		//zazhgem LD
//		GPIOC->ODR |= GPIO_ODR_6;//
//		
//		for(i=0;i<100000L;i++);		//wait
//		
//		//gasit LD
//		GPIOC->ODR &= ~GPIO_ODR_6;
//		
//		for(i=0;i<100000L;i++);		//wait
//	}

	while(1)
	{
		sos(1);
		sos(1);
		sos(1);
		sos(3);
		sos(3);
		sos(3);
		sos(1);
		sos(1);
		sos(1);
		for(i=0;i<300000L;i++);		//wait
		
	}

}

void sos(int t)
{
		static uint32_t i;
		//zazhgem LD
		GPIOC->ODR |= GPIO_ODR_6;//
		
		for(i=0;i<t*100000L;i++);		//wait
		
		//gasit LD
		GPIOC->ODR &= ~GPIO_ODR_6;
		
		for(i=0;i<t*100000L;i++);		//wait
	
}

