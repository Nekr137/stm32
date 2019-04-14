
#include "stm32f0xx.h"                  // Device header

//MACROS

		//MY BUTTON MACROS
		#define BUTTON_IN()					(GPIOA->IDR & GPIO_IDR_0)
		#define PortAClocking()			(RCC->AHBENR |= RCC_AHBENR_GPIOAEN)
		#define PortCClocking()			(RCC->AHBENR |= RCC_AHBENR_GPIOCEN)

		//MY LED macros
		#define SETLED6()						(GPIOC->BSRR = GPIO_BSRR_BS_6)
		#define SETLED9()						(GPIOC->BSRR = GPIO_BSRR_BS_9)
		#define SETLED7()						(GPIOC->BSRR = GPIO_BSRR_BS_7)
		#define SETLED8()						(GPIOC->BSRR = GPIO_BSRR_BS_8)
		#define RESETLED6()					(GPIOC->BSRR = GPIO_BSRR_BR_6)
		#define RESETLED9()					(GPIOC->BSRR = GPIO_BSRR_BR_9)
		#define RESETLED7()					(GPIOC->BSRR = GPIO_BSRR_BR_7)
		#define RESETLED8()					(GPIOC->BSRR = GPIO_BSRR_BR_8)
		




void SysTick_Handler()
{
	
}


int main()
{
		//preparation
				SysTick_Config(SystemCoreClock/1000);// 1000HZ TICK
			
				MyClocking();
				MyLEDConfigurationRegisters();
		
		//start
				while(1)
				{
							Regime();				
				}
}




void MyClocking()
{
		//RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
		PortAClocking();
	  //RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
		PortCClocking();
}

void MyLEDConfigurationRegisters()
{
/// Configuration registers LED

	//GPIO port mode register
			GPIOC->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7 | GPIO_MODER_MODER8 | GPIO_MODER_MODER9);
			GPIOC->MODER |= (GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0);
	
	//GPIO port output type register (GPIOx_OTYPER)
			GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7 | GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9);
					
	//GPIO port output speed register (GPIOx_OSPEEDR) (x = A..F)
			GPIOC->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7 | GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9);
					
	//GPIO port pull-up/pull-down register (GPIOx_PUPDR) (x = A..F)
			GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR7 | GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9);
///
}