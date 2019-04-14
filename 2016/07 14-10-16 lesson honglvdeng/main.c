//hong lv deng

//4 regime: if button is pressed -> 3 regime!!

#include "stm32f0xx.h"                  // Device header


#define PortAClocking()				(RCC->AHBENR |= RCC_AHBENR_GPIOAEN)
#define PortCClocking()				(RCC->AHBENR |= RCC_AHBENR_GPIOCEN)

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

int Regime = 1;
int tempRegime = 0;
int FlagSecCnt = 0;
int SwtchRegimeFlg = 1;
int FLAG_LED_red = 1;
int FLAG_LED_green = 1;
int FLAG_LED_yellow = 1;
int FLAG_LED_blue = 1;

int ms = 0;
int BipCount = 0;
int SecCnt = 0;
int CountLEDred = 0;
int CountLEDgreen = 0;
int CountLEDyellow = 0;
int CountLEDblue = 0;

int BlinkingCount = 0;//for blinking friquency
int SetBlinkingCount = 0;//every function must define this value
int BlinkingFlag = 0;//if it "Up" -> you can f.e. turn on LED in your function

int MyButtonState = 0;
int ChangeRegimeFlag = 0;

void MyClocking(void);
void MyLEDConfigurationRegisters(void);
void Regime1(void);
void Regime2(void);
void Regime3(void);
void Regime4(void);
void Regime5(void);

void Regime2_(void);
void Regime4_(void);

void SysTick_Handler()
{
	ms++;
	//---seconds--//
	if(SecCnt<1000)
	{
		SecCnt++;
	}
	else
	{
		SecCnt = 0;
		FlagSecCnt = On;
	}
	//--seconds--//
	
	//--button--//
	MyButtonState = BUTTON_IN();
		if(MyButtonState == 1)
		{
			BipCount++;
		}
		else
		{
			BipCount = 0;
		}
		if(BipCount == 30)
		{
			ChangeRegimeFlag = Up;
		}
	//--button--//
		
	//--blinking--//
		if(++BlinkingCount >= SetBlinkingCount)//ms
		{
			BlinkingCount = 0;
			if(BlinkingFlag == 1)
			{
				BlinkingFlag = 0;
			}
			else
			{
				BlinkingFlag = 1;
			}
		}
	//--blinking--//
		

	
	//----LED LIGHTING----//
		if(CountLEDred > 0)
		{
					CountLEDred--;
					SETLEDred();
					FLAG_LED_red = Down;//flag doesn't permit the "countLED6" var changing in the "main"
		}
		else
		{
				RESETLEDred();
				FLAG_LED_red = Up; //flag is prepared to change the "countLED6" in "main"
		}
		if(CountLEDgreen > 0)
		{
					CountLEDgreen--;
					SETLEDgreen();
					FLAG_LED_green = Down;
		}
		else
		{
				RESETLEDgreen();
				FLAG_LED_green = Up;
		}
		if(CountLEDyellow > 0)
		{
					CountLEDyellow--;
					SETLEDyellow();
					FLAG_LED_yellow = Down;
		}
		else
		{
				RESETLEDyellow();
				FLAG_LED_yellow = Up;
		}
		if(CountLEDblue > 0)
		{
					CountLEDblue--;
					SETLEDblue();
					FLAG_LED_blue = Down;
		}
		else
		{
				RESETLEDblue();
				FLAG_LED_blue = Up;
		}	
		//----end LED LIGHTING-----//
}//systick




int main()
{
	SysTick_Config(SystemCoreClock/1000);// 1000HZ TICK
	MyClocking();
	MyLEDConfigurationRegisters();
	
	while(1)
	{
		switch(Regime)
		{
			case 1:
			Regime1();
				break;

			case 2:
			Regime2();
				break;
			case 3:
			Regime3();
				break;

			case 4:
				Regime4();
				break;

			case 5:
				Regime5();
				break;
		}//switch
	}//while
}//main

void Regime1(void)
{
	ChangeRegimeFlag = 0;
	while(ms<3000)
	{
		if(FLAG_LED_red == 1)
		{
			CountLEDred = 3000;
		}
		if(ChangeRegimeFlag == 1)
		{
			ms = 2900;
			CountLEDred = 100;
			ChangeRegimeFlag = 0;
		}
	}
	ms = 0;
	Regime = 2;
}

void Regime2(void)//my new blinking code is more interesting and flexible
{
	while(ms<1000)
	{
		if(FLAG_LED_red == 1)
		{
			CountLEDred = 1000;
		}
		if(FLAG_LED_yellow == 1)
		{
			CountLEDyellow = 1000;
		}
	}
	ms = 0;
	Regime = 3;	
}

void Regime3(void)
{
	int temp = 0;
	ChangeRegimeFlag = 0;
	while(ms<3000)
	{
		if(FLAG_LED_green == 1)
		{
			CountLEDgreen = 3000;
		}
	}
	ms = 0;
	Regime = 4;	
	
	if(ChangeRegimeFlag == 1)
	{
		ms = 0;
		CountLEDgreen = 3000;
		ChangeRegimeFlag = 0;
		temp = 1;
	}
	if(temp == 1)
	{
		Regime = 3;
	}	
}


void Regime5(void)
{
	while(ms<1000)
	{
		if(FLAG_LED_yellow == 1)
		{
			CountLEDyellow = 1000;
		}
	}
	ms = 0;
	Regime = 1;		
}

void Regime4(void)//modifiyed regime 4
{
	int ButtonPressedRegimeChanging = 0;
	int FlagStop = Down;//exit from while (from blinking)
	SetBlinkingCount = 100;
	ms = 0;
	while(FlagStop != 1)
	{
		//--check the ms or pressed buttton
		if(ms>3000)
		{
			FlagStop = Up;
		}
		if(ChangeRegimeFlag == 1)
		{
			FlagStop = Up;
			ChangeRegimeFlag = Down;
			ButtonPressedRegimeChanging = 1;
		}
		//-- "if" responsible for blinking  --//
		if(BlinkingFlag == 1)
		{
			SETLEDgreen();
		}
		else
		{
			RESETLEDgreen();
		}
	}
	ms = 0;
	Regime = 5;
	if(ButtonPressedRegimeChanging == 1)
	{
		Regime = 3;
	}
}


void Regime4_()//this function is too long and unflexible
{
	int temp = 0;//for the "if". If temp == 1 -> Regime = 2
	while(ms<500)
	{
		if(FLAG_LED_green == 1)
		{
			CountLEDgreen = 500;
		}
		if(ChangeRegimeFlag == 1)//check the button
		{
			ms = 3001;
			CountLEDgreen = 10;
			ChangeRegimeFlag = 0;
			temp = 1;
		}	
	}
	while(ms<1000)
	{
		if(ChangeRegimeFlag == 1)//check the button
		{
			ms = 3001;
			CountLEDgreen = 10;
			ChangeRegimeFlag = 0;
			temp = 1;
		}	
	}
	ms = 0;
	while(ms<1500)
	{
		if(FLAG_LED_green == 1)
		{
			CountLEDgreen = 500;
		}
		if(ChangeRegimeFlag == 1)//check the button
		{
			ms = 3001;
			CountLEDgreen = 10;
			ChangeRegimeFlag = 0;
			temp = 1;
		}	
	}
	while(ms<2000)
	{
		if(ChangeRegimeFlag == 1)//check the button
		{
			ms = 3001;
			CountLEDgreen = 10;
			ChangeRegimeFlag = 0;
			temp = 1;
		}	
	}
	while(ms<2500)
	{
		if(FLAG_LED_green == 1)
		{
			CountLEDgreen = 500;
		}
		if(ChangeRegimeFlag == 1)//check the button
		{
			ms = 3001;
			CountLEDgreen = 10;
			ChangeRegimeFlag = 0;
			temp = 1;
		}	
	}
	while(ms<3000)
	{
		if(ChangeRegimeFlag == 1)//check the button
		{
			ms = 3001;
			CountLEDgreen = 10;
			ChangeRegimeFlag = 0;
			temp = 1;
		}	
	}
	ms = 0;
	Regime = 5;	
	if(temp == 1)
	{
		Regime = 3;
	}		
}

void MyClocking()
{
		//RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
		PortAClocking();
	  //RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
		PortCClocking();
}// END OF MyClocking()

void MyLEDConfigurationRegisters()
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
}//END OF void MyLEDConfigurationRegisters();


