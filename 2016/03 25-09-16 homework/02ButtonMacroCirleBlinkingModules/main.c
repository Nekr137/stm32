//****************************************************************************************
//INCLUDE SECTION
//****************************************************************************************

#include "header.h"

//****************************************************************************************
//VARIABLES DEFENITION SECTION
//****************************************************************************************

//----------------------------------------------------------------------------------------
//GLOBAL
//----------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------
//LOCAL
//----------------------------------------------------------------------------------------

	uint32_t MyButtonState = 0;
	uint32_t MyButtonMode = 0;
	//const int MyModes = 3;							//LEDS Regimes
	int MyCount = 0;											//the time at which the LED lights
	int MyLEDNumber = 0;
	double MyJitterTime = 0.01;
	
	
//*****************************************************************************************
//SECTION OF LOCAL FUNCTIONS PROTOTYPES
//*****************************************************************************************

	void MyClocking();										//taktirovaniye
	void MyLEDConfigurationRegisters();		//registers of configuration

	void MyDelay();
	void MyMainFn();

	void MyClockWiseLEDSRotation();
	void MyUnderClockWiseLEDSRotation();
	void MyLEDSBlinking();
	void MyLEDSTurnOff();

	void SetAllLEDS();
	void ResetAllLEDS();
	
//*****************************************************************************************
//SYSTICK
//*****************************************************************************************

		void SysTick_Handler()
		{
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
					
					default:
							MyLEDSBlinking();
							break;
				}
		}

//*****************************************************************************************
//MAIN
//*****************************************************************************************

		int main()
		{
				//preparation
						SysTick_Config(SystemCoreClock/1000);// 1000HZ TICK
					
						MyClocking();
						MyLEDConfigurationRegisters();
				
				//start
						MyMainFn();
		}

//*****************************************************************************************
//SECTION OF FUNCTION DESCRIPTION
//*****************************************************************************************

void MyMainFn()
{
		while(1)
		{
			//check the button
					MyButtonState = BUTTON_IN();
			
			//if the button is pressed the mode changes
					if (MyButtonState == 1) 
					{
							if (MyButtonMode != MyModes)
							{
									MyButtonMode = MyButtonMode + 1;
							}
							else 
							{
									MyButtonMode = 0;
							}
					}
		}
}


void MyClockWiseLEDSRotation()
{
//toggle
	if (MyCount==0)
	{
			MyCount = 100;
			switch (MyLEDNumber)
			{
				case 6:
					MyLEDNumber = 8;
					break;
				case 8:
					MyLEDNumber = 7;
					break;
				case 7:
					MyLEDNumber = 9;
					break;
				case 9:
					MyLEDNumber = 6;
					break;
			}								
	}	
//set LED
	switch (MyLEDNumber)
	{
		case 6:
			SETLED6(); 
			MyCount = MyCount-1;
			break;
		case 8:
			SETLED8();
			MyCount = MyCount-1;
			break;
		case 7:
			SETLED7();
			MyCount = MyCount-1;
			break;
		case 9:
			SETLED9();
			MyCount = MyCount-1;
			break;
	}	
}

void MyUnderClockWiseLEDSRotation()
{
//toggle
	if (MyCount==0)
	{
			MyCount = 100;
			switch (MyLEDNumber)
			{
				case 6:
					MyLEDNumber = 9;
					break;
				case 9:
					MyLEDNumber = 7;
					break;
				case 7:
					MyLEDNumber = 8;
					break;
				case 8:
					MyLEDNumber = 6;
					break;
			}								
	}	
//set LED
	switch (MyLEDNumber)
	{
		case 6:
			SETLED6();  
			MyCount = MyCount-1;
			break;
		case 9:
			SETLED9();
			MyCount = MyCount-1;
			break;
		case 7:
			SETLED7();
			MyCount = MyCount-1;
			break;
		case 8:
			SETLED8();
			MyCount = MyCount-1;
			break;
	}		
}

void MyLEDSBlinking()
{
		int MySwitch = NOTLIGHT;
	//toggle
		if (MyCount==0)
		{
				MyCount = 100;
				if(MySwitch==NOTLIGHT)
				{
					MySwitch=LIGHT;
				}
				else
				{
					MySwitch=NOTLIGHT;
				}
		}	
		
		if (MySwitch == NOTLIGHT)
		{
				SetAllLEDS();
				MyCount = MyCount-1;
		}
		else
		{
				ResetAllLEDS();
				MyCount = MyCount-1;
		}	
}

void MyLEDSTurnOff()
{
		ResetAllLEDS();
}

void SetAllLEDS()
{
		SETLED6();
		SETLED7();
		SETLED8();
		SETLED9();
}

void ResetAllLEDS()
{
		RESETLED6();
		RESETLED7();
		RESETLED8();
		RESETLED9();
	
}

void MyDelay(double DelayTime)//not used
{		
		int i;
	
    for (i=0; i<DelayTime*100000; i++ )
    {
//				// make a delay to avoid jitter
//				MyButtonState = BUTTON_IN();
//				if (BUTTON_STATE == 1)
//				{
//						break;
//				}	
    }
}


void MyClocking()
{
		RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
		RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
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

//*****************************************************************************************
//END OF FILE
//*****************************************************************************************

