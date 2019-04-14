
#include "MyLED.h"

void LEDconfig(void)
{
			RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	
			GPIOC->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7 | GPIO_MODER_MODER8 | GPIO_MODER_MODER9);
			GPIOC->MODER |= (GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0);
			GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7 | GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9);
			GPIOC->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7 | GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9);
			GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR7 | GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9);
}

void LEDshowNum(uint32_t ADCdata)
{
	
		RESETLEDgreen();
		RESETLEDred();
		RESETLEDyellow();
		RESETLEDblue();
		
		if(ADCdata<819){}
		if((ADCdata>819) && (ADCdata<1638)){SETLEDblue();}
		if((ADCdata>1638) && (ADCdata<2457)){SETLEDyellow();	SETLEDblue();}
		if((ADCdata>2457) && (ADCdata<3276)){SETLEDred();SETLEDyellow();	SETLEDblue();}
		if((ADCdata>3276)){SETLEDred();SETLEDyellow();	SETLEDblue();SETLEDgreen();}
	}
