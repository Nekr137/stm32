// Tree Timer 19/12/16
#include "stm32f0xx.h"                  // Device header

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

uint32_t msTick = 0;
uint32_t sTick = 0;
uint32_t mTick = 0;
uint32_t hTick = 0;
uint32_t dTick = 0;
uint32_t MyButtonState = 0;
uint32_t MyButtonCount = 0;
uint32_t Set2ZeroFlag = 0;
uint32_t EndOfCountFlag = 0;

void MyLEDConfigurationRegisters(void);
void MyTimer(void);
void MyModeSelect(void);
void MyResetAllLEDs(void);

void SysTick_Handler()
{
	msTick++;
	
	//to control the button state
	MyButtonState = BUTTON_IN();
	if(MyButtonState == 1)
	{
		MyButtonCount++;
	}
	else
	{
		MyButtonCount = 0;
	}
	if(MyButtonCount == 3000)
	{
		Set2ZeroFlag = 1;
	}
	///
}



int main()
{
	SysTick_Config(SystemCoreClock/1000);//1000HzTick
	MyLEDConfigurationRegisters();
	while(1)
	{
		MyTimer();//count the ms sec min hours days
		if(Set2ZeroFlag == 1)
		{
			msTick = 0; sTick = 0; mTick = 0; hTick = 0; dTick = 0;EndOfCountFlag = 0;
			Set2ZeroFlag = 0;
		}
		MyModeSelect();
	}
	
}

void MyModeSelect(void)
{
	MyResetAllLEDs();
	SETLEDred();

	if((hTick<12)&&(dTick<1))
	{
		SETLEDgreen();
		RESETLEDblue();
		RESETLEDyellow();
	}
	if((hTick>=12) && (hTick<24) && (dTick<1))
	{
		RESETLEDyellow();
		SETLEDblue();
		RESETLEDgreen();
	}
	if((dTick>=1))
	{
		RESETLEDblue();
		RESETLEDgreen();
		SETLEDyellow();
	}
}

void MyResetAllLEDs(void)
{
	RESETLEDblue();
	RESETLEDgreen();
	RESETLEDred();
	RESETLEDyellow();
}

void MyTimer(void)
{
	if(msTick>1000)
	{
		msTick = msTick-1000;// I think value can not be reset because it can be greater then 1000
		sTick++;
	}
	if(sTick>60)
	{
		sTick = sTick-60;
		mTick++;
	}
	if(mTick>60)
	{
		mTick = 0;
		hTick++;
	}
	if(hTick>24)
	{
		hTick = 0;
		dTick++;
	}
}

void MyLEDConfigurationRegisters(void)
{
	//Port C Clocking
			RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	//Port A Clocking
			RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	//GPIO port mode register
			GPIOC->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7 | GPIO_MODER_MODER8 | GPIO_MODER_MODER9);
			GPIOC->MODER |= (GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0);
	
	//GPIO port output type register (GPIOx_OTYPER)
			GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7 | GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9);
					
	//GPIO port output speed register (GPIOx_OSPEEDR) (x = A..F)
			GPIOC->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7 | GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9);
					
	//GPIO port pull-up/pull-down register (GPIOx_PUPDR) (x = A..F)
			GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR7 | GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9);
}//

