#include "stm32f0xx.h"
			

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
		
		//#define SETALLLEDSMACRO()   (SETLED6(); SETLED7(); SETLED8(); SETLED9();)
		//#define RESETALLLEDSMACRO() (RESETLED6();RESETLED7();RESETLED8();RESETLED9();)
		
		//MY CONSTANTS
		#define MyModes 3											//LEDS Regimes
		
		//bool
		#define PRESSED 1 
		#define UNPRESSED 0 
		#define LIGHT 1
		#define NOTLIGHT 0
		#define Up 1
		#define Down 0
		
		

//variables
int Count = 0;								//for "seconds" timer
int SecondsFLAG = 0;					//one second is over
int EnterCaseFlag = 0;				//entered to the "case". 
int temp;
int ModeHasBeenChanged = 1;		//the mode (work regime) just has been changed
int CountLED6 = 0;
int CountLED7 = 0;
int CountLED8 = 0;
int CountLED9 = 0;

int FlagLED6 = 0;
int FlagLED8 = 0;
int FlagLED7 = 0;
int FlagLED9 = 0;

int LEDNumber = 0;						//which LED just lighted

int MyButtonMode = 0;
int MyButtonState = 0;
int j = 0;
int PIP_CNT = 0;
int beep_cnt;
int pip_on;
int pip_off;
int ButtonPushed = 1;

int i = 0;

void MyLEDConfigurationRegisters();
void MyClocking();

void MyClockWiseLEDSRotation();
void MyUnderClockWiseLEDSRotation();
void MyLEDSTurnOff();
void MyLEDSBlinking();

void ModeOperation();
void Regime();

void SysTick_Handler()
{
		//check the button
		MyButtonState = BUTTON_IN();
		j = MyButtonState; 
	
	
	
		///////////////////
		//beep_mafaka
		
		//j = GPIOA->IDR & 1;// the last is responsible for the Bib count setting
		if (j == 1)
		{
			PIP_CNT++;
		}
		else 
		{
			PIP_CNT=0;
		}
		if (PIP_CNT == 30)
		{
			ButtonPushed = 1;
		}
		//////////////////
		
		//"seconds" timer
		if(++Count >= 1000)
		{
					Count = 0;
					SecondsFLAG = 1;
		}
		
		//led6timer
		if(CountLED6 > 0)
		{
					CountLED6--;
					SETLED6();
		}
		else
		{
				RESETLED6();
		}
		
		//led7timer
		if(CountLED7 > 0)
		{
					CountLED7--;
					SETLED7();
		}
		else
		{
				RESETLED7();
		}
		
		//led8timer
		if(CountLED8 > 0)
		{
					CountLED8--;
					SETLED8();
		}
		else
		{
				RESETLED8();
		}
		
		//led9timer
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
		
		//start
				while(1)
				{
							Regime();				
				}
}



void Regime()	//operations with button and regimes (functions)
{
		//if the button is pressed the mode changes
		if (ButtonPushed == Up) 
		{
				//mode changing
				if (MyButtonMode != MyModes)
				{
						MyButtonMode = MyButtonMode + 1;
				}
				else 
				{
						MyButtonMode = 0;
				}
				//signal that mode has been changed
				ModeHasBeenChanged = 1;
				ButtonPushed = Down;
		}
		
		
		//REGIME!!!
		switch (MyButtonMode) 
		{
			case 1:
					MyClockWiseLEDSRotation();	
					break;
			
			case 2:
					MyUnderClockWiseLEDSRotation();
					break;
			
			case 3:
					MyLEDSTurnOff();
					break;
			
			case 0:
					MyLEDSBlinking();
					break;
		}
}

void MyClockWiseLEDSRotation()
{
		//SECONDs FLAG
		if(SecondsFLAG == Up)
		{		
			SecondsFLAG = Down;
			if(temp == 3)
			{
					temp = 0;
			}
			else
			{
					temp++;
			}
		}		
		
		//if the mode just has been changed:
		if(ModeHasBeenChanged == Up)
		{
			ModeHasBeenChanged = Down;
			temp = 0;
			EnterCaseFlag = Up;
		}
		
		//case
		switch(temp)
		{
			case 0:
				if(EnterCaseFlag == Up)
				{
						CountLED6 = 500;
						EnterCaseFlag = Down;
				}
				break;
			case 1:
				if(EnterCaseFlag == Down)
				{
						CountLED9 = 500;
						EnterCaseFlag = Up;
				}
				break;
			case 2:
				if(EnterCaseFlag == Up)
				{
						CountLED7 = 500;
						EnterCaseFlag = Down;
				}
				break;				
			case 3:
				if(EnterCaseFlag == Down)
				{
						CountLED8 = 500;
						EnterCaseFlag = Up;
				}
				break;
		}
}


void MyUnderClockWiseLEDSRotation()
{
	//SECONDs FLAG
	if(SecondsFLAG == Up)
	{		
		SecondsFLAG = Down;
		if(temp == 3)
		{
				temp = 0;
		}
		else
		{
				temp++;
		}
	}		
	
	//if the mode just has been changed 
	if(ModeHasBeenChanged == Up)
	{
		ModeHasBeenChanged = Down;
		switch (LEDNumber)//0123 -> 6978 - clock wise
		{
			case 0:
				temp = 3;
				break;
			case 1:
				temp = 0;
				break;
			case 2:
				temp = 1;
				break;
			case 3:
				temp = 2;
				break;
		}
	}

	//case
	switch(temp)
	{
		case 0:
			if(EnterCaseFlag == Up)
			{
					CountLED6 = 500;
					EnterCaseFlag = Down;
			}
			break;
		case 1:
			if(EnterCaseFlag == Down)
			{
					CountLED8 = 500;
					EnterCaseFlag = Up;
			}
			break;
		case 2:
			if(EnterCaseFlag == Up)
			{
					CountLED7 = 500;
					EnterCaseFlag = Down;
			}
			break;				
		case 3:
			if(EnterCaseFlag == Down)
			{
					CountLED9 = 500;
					EnterCaseFlag = Up;
			}
			break;
	}
}

void MyLEDSTurnOff()
{
		RESETLED6();
		RESETLED7();
		RESETLED8();
		RESETLED9();
}
void MyLEDSBlinking()
{
	//SECONDs FLAG
	if(SecondsFLAG == Up)
	{		
		SecondsFLAG = Down;
		if(temp == 1)
		{
				temp = 0;
		}
		else
		{
				temp++;
		}
	}		
	
	//if the mode just has been changed 
	if(ModeHasBeenChanged == Up)
	{
			temp = 0;
			ModeHasBeenChanged = Down;
			EnterCaseFlag = Down;
	}
	
	//switch
	switch(temp)
	{
		case 0:
			if(EnterCaseFlag == Down)
			{
				CountLED6 = 500;
				CountLED7 = 500;
				CountLED8 = 500;
				CountLED9 = 500;
				EnterCaseFlag = Up;
			}
			break;
			
		case 1:
			if(EnterCaseFlag == Up)
			{
//				RESETLED6();
//				RESETLED7();
//				RESETLED8();
//				RESETLED9();
				EnterCaseFlag = Down;
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
