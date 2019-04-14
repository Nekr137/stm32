//indicator


#include "stm32f0xx.h"                  // Device header


#define DELAY 10000

#define SetRS()			(GPIOB->BSRR = GPIO_BSRR_BS_12)
#define ResetRS()		(GPIOB->BSRR = GPIO_BSRR_BR_12)
#define SetRW()			(GPIOB->BSRR = GPIO_BSRR_BS_3)
#define ResetRW()		(GPIOB->BSRR = GPIO_BSRR_BR_3)

#define Delay()			(for(n = 0; n<DELAY; n++))

void Init(void);
void LCD_Config(void);
void LCD_WriteData(uint32_t data);
void LCD_WriteCMD(void);
void LCD_Wait(void);
void LCD_Pulse(void);

uint32_t data;


void SysTick_Handler()
{
}


int main()
{
	SysTick_Config(SystemCoreClock/1000);// 1000HZ TICK

			Init();
			while(1)
			{
			}
	
}

void Init(void)
{
	//light
			RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
			GPIOC->MODER &= ~(GPIO_MODER_MODER6);
			GPIOC->MODER |= (GPIO_MODER_MODER6_0);
			GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_6);
			GPIOC->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR6);
			GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR6);	
	
			GPIOC->BSRR = GPIO_BSRR_BS_6;	
}

void LCD_Config(void)//clocking and pins config
{
			RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
			RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
			
			GPIOB->MODER &= ~(GPIO_MODER_MODER3 | GPIO_MODER_MODER4 | GPIO_MODER_MODER5 | GPIO_MODER_MODER6 | GPIO_MODER_MODER7 | GPIO_MODER_MODER12);
			GPIOB->MODER |= (GPIO_MODER_MODER3_0 | GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER12_0);
			GPIOB->OTYPER &= ~(GPIO_OTYPER_OT_3 | GPIO_OTYPER_OT_4 | GPIO_OTYPER_OT_5 | GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7 | GPIO_OTYPER_OT_12);
			GPIOB->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR3 | GPIO_OSPEEDER_OSPEEDR4 | GPIO_OSPEEDER_OSPEEDR5 | GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7 | GPIO_OSPEEDER_OSPEEDR12);
			GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR3 | GPIO_PUPDR_PUPDR4 | GPIO_PUPDR_PUPDR5 | GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR7 | GPIO_PUPDR_PUPDR12);	
	
			GPIOC->MODER &= ~(GPIO_MODER_MODER4);
			GPIOC->MODER |= (GPIO_MODER_MODER4_0);
			GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_4);
			GPIOC->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR4);
			GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR4);	
}

void LCD_WriteData(uint32_t data)
{
	LCD_Wait();
	//rs -> 1
	
}

void LCD_WriteCMD(void)
{
	///rs->0
	
	
	
}

void LCD_Wait(void)
{
		uint32_t ExitWhileFlag = 0;
		uint32_t tempRead = 0;
		//read status
		//bit bisy 
		//while tupim wait osvoboditcya (pb7)
		//read register
	//INPUT
	//rs - 0
	//rw - 1
	//en - dergaem starshaya tetrada
	//obratno na vihod
	//rw - 0;
	
			RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
			RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
			
			GPIOB->MODER &= ~(GPIO_MODER_MODER3 | GPIO_MODER_MODER4 | GPIO_MODER_MODER5 | GPIO_MODER_MODER6 | GPIO_MODER_MODER7 | GPIO_MODER_MODER12);
			GPIOC->MODER &= ~(GPIO_MODER_MODER4);
	
			ResetRS();
			SetRW();

			while(ExitWhileFlag != 1)
			{
					LCD_Pulse();
					tempRead = GPIOB->IDR & GPIO_IDR_7;
					LCD_Pulse();
					tempRead = GPIOB->IDR & GPIO_IDR_7;
					if(tempRead == 0)
					{
							ExitWhileFlag = 1;
							ResetRW();
					}
			}	
			ResetRW();
}

void LCD_Pulse(void)
{
		uint32_t n;
		for(n = 0; n<DELAY; n++)
		{
		}
		
		GPIOC->BSRR = GPIO_BSRR_BS_4;
		
		for(n = 0; n<DELAY; n++)
		{
		}
		
		GPIOC->BSRR = GPIO_BSRR_BR_4;
		
		for(n = 0; n<DELAY; n++)
		{
		}
} 


void LCD_WhriteHalf(uint32_t data)
{
			GPIOC->BSRR = (GPIO_BSRR_BR_4 | GPIO_BSRR_BR_5 | GPIO_BSRR_BR_6 | GPIO_BSRR_BR_7);
			GPIOC->BSRR = data<<4;
}
