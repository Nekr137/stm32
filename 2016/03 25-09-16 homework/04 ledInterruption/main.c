//program which turn on and torn off the 5LED using interruption

#include "stm32f0xx.h"
			
//MACROS
		#define BUTTON_IN()					(GPIOA->IDR & GPIO_IDR_0)
		#define PortAClocking()			(RCC->AHBENR |= RCC_AHBENR_GPIOAEN)
		#define PortCClocking()			(RCC->AHBENR |= RCC_AHBENR_GPIOCEN)

		//MY LED macros
		#define SETLED6()						(GPIOC->BSRR = GPIO_BSRR_BS_6)
		#define RESETLED6()					(GPIOC->BSRR = GPIO_BSRR_BR_6)
		
		//MY CONSTANTS
		#define LIGHT 1
		#define NOTLIGHT 0
		
//variables
		uint32_t MyButtonMode = 0;
		uint32_t MyButtonState = 0;
		int Count = 1000;
		int LEDmode = 0;
		

//functions
		void MyClocking();										//taktirovaniye
		void MyLEDConfigurationRegisters();		//registers of configuration
		void MyMainFn();
		void MyLEDTrigger();
		
void SysTick_Handler()
{
		if(++Count >= 1000)
		{
			Count = 0;
			MyLEDTrigger();
		}
}

int main()
{
		//preparation
				SysTick_Config(SystemCoreClock/1000);// 1000HZ TICK
			
				MyClocking();
				MyLEDConfigurationRegisters();
		
		//start
				MyMainFn();

}

void MyMainFn()
{
		while(1)
		{
			
		}
}

void MyLEDTrigger()
{
		if(LEDmode == LIGHT)
		{
			LEDmode = NOTLIGHT;
			RESETLED6();
		}
		else
		{
			LEDmode = LIGHT;
			SETLED6();
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