
#include "stm32f0xx.h"                  // Device header


//MY BUTTON MACROS

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

#define PA45()							(GPIOA->IDR & (GPIO_IDR_4 | GPIO_IDR_5))
#define PA15S()							(GPIOA->BSRR = GPIO_BSRR_BS_15)
#define PC12S()							(GPIOC->BSRR = GPIO_BSRR_BS_12)
#define PA15R()							(GPIOA->BSRR = GPIO_BSRR_BR_15)
#define PC12R()							(GPIOC->BSRR = GPIO_BSRR_BR_12)		

#define PRESSED 		1 
#define UNPRESSED 	0 
#define LIGHT 			1
#define NOTLIGHT 		0
#define Up 					1
#define Down 				0


//VARIABLES
volatile uint32_t word = 0;

int Count = 0;

int CountLED6 = 0;
int CountLED7 = 0;
int CountLED8 = 0;
int CountLED9 = 0;

int FLAG_LED_6 = 1;//if '1' -> allowed to change the "countLEDx" variable
int FLAG_LED_7 = 1;
int FLAG_LED_8 = 1;
int FLAG_LED_9 = 1;

//FUNCTIONS
void MyClocking(void);
void MyLEDConfigurationRegisters(void);
void MyKeybardConfig(void);

void SysTick_Handler()
{
		uint32_t in1,in2,temp;
		volatile int k = 0;

   	//whrite 10
		PA15S();
		PC12R();
			for(k = 0; k<20 ; k++)
		in1 = PA45() >> 4;

		//whrite 01
		PA15R();
		PC12S();
		for(k = 0; k<20 ; k++)
		in2 = PA45() >> 2;
	
		PA15R();
		PC12R();
		
		temp = (in1 | in2);
		
		if(temp)
		{
			Count++;
		}
		else
		{
			Count = 0;
		}

		if(Count == 30)
		{
			word = temp;
		}
		
		
		if(CountLED6 > 0)
		{
					CountLED6--;
					SETLED6();
				//	FLAG_LED_6 = Down;//flag doesn't permit the "countLED6" var changing in the "main"
		}
		else
		{
				RESETLED6();
			//	FLAG_LED_6 = Up; //flag is prepared to change the "countLED6" in "main"
		}
		if(CountLED7 > 0)
		{
					CountLED7--;
					SETLED7();
		}
		else
		{
				RESETLED7();
		}
		if(CountLED8 > 0)
		{
					CountLED8--;
					SETLED8();
		}
		else
		{
				RESETLED8();
		}
		if(CountLED9 > 0)
		{
					CountLED9--;
					SETLED9();
		}
		else
		{
				RESETLED9();
		}	
}


int main()
{
		//preparation
				SysTick_Config(SystemCoreClock/1000);// 1000HZ TICK
			
				MyClocking();
				MyLEDConfigurationRegisters();
				MyKeybardConfig();
		
		//start
				while(1)
				{
							if ((word & 0x1) == 0x1)		//now this task is much easier. The program does't go into the func if the bit is not set!
							{														//there is no need for any flags!!!
									CountLED6 = 1000;
									word &= ~(0x1);					//word = (word & ~(0x1));
							}
//							else											//the LED reseting is realised in the sistick handler!!!
//							{
//									if (FLAG_LED_6 == Up)
//									RESETLED6();
//							}
							
							if ((word & 0x2) == 0x2)
							{
									CountLED7 = 1000;
									word &= ~(0x2);
							}
							
							if ((word & 0x4) == 0x4)
							{
									CountLED9 = 1000;
									word &= ~(0x4);
							}

							if ((word & 0x8) == 0x8)
							{
									CountLED8 = 1000;
								  word &= ~(0x8);
							}
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

void MyKeybardConfig()
{
		RCC->AHBENR |= (RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOCEN);
	
		GPIOA->MODER &= ~(GPIO_MODER_MODER4 | GPIO_MODER_MODER5);
		GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR4 | GPIO_PUPDR_PUPDR5);
		GPIOA->PUPDR |= (GPIO_PUPDR_PUPDR4_1 | GPIO_PUPDR_PUPDR5_1);
	
		GPIOA->MODER &= ~(GPIO_MODER_MODER15);
		GPIOA->MODER |= (GPIO_MODER_MODER15_0);
		GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_15);
		GPIOA->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR15);
		GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR15);	
	
		GPIOC->MODER &= ~(GPIO_MODER_MODER12);
		GPIOC->MODER |= (GPIO_MODER_MODER12_0);
		GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_12);
		GPIOC->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR12);
		GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR12);		
}

