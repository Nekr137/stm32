
/* nekr 04 11 2017 */
/* if HSEBYP visnet na "while" */ 

#include "stm32f0xx.h"                  // Device header

#define SETLEDred()						(GPIOC->BSRR = GPIO_BSRR_BS_6)
#define SETLEDgreen()					(GPIOC->BSRR = GPIO_BSRR_BS_9)
#define SETLEDblue()					(GPIOC->BSRR = GPIO_BSRR_BS_7)
#define SETLEDyellow()				(GPIOC->BSRR = GPIO_BSRR_BS_8)
#define RESETLEDred()					(GPIOC->BSRR = GPIO_BSRR_BR_6)
#define RESETLEDgreen()				(GPIOC->BSRR = GPIO_BSRR_BR_9)
#define RESETLEDblue()				(GPIOC->BSRR = GPIO_BSRR_BR_7)
#define RESETLEDyellow()			(GPIOC->BSRR = GPIO_BSRR_BR_8)

#define ResetAllLEDs()				(GPIOC->BSRR = (GPIO_BSRR_BR_6|GPIO_BSRR_BR_7|GPIO_BSRR_BR_8|GPIO_BSRR_BR_9))


uint32_t tick = 0;
uint8_t flag_yellow_LED = 0;

void PLLHSEBYPConfig(void);
void PLLHSIConfig(void);
void LEDConfig(void);
uint8_t Trig(uint8_t flag);
void DefineYellow(void);


void SysTick_Handler(void)
{
	tick++;
}

int main(void)
{
	//PLLHSIConfig();
	PLLHSEBYPConfig();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/1000); /* 1000Hz tick */

	LEDConfig();
	SETLEDgreen();
	while(1)
	{
		if(tick>500)
		{
			tick = 0;
			flag_yellow_LED = Trig(flag_yellow_LED);
			DefineYellow();		
		}
	}
	
}

void PLLHSIConfig(void)
{
			if ((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_PLL) 				/* (1)  Test if PLL is used as System clock */
			{          
				RCC->CFGR &= (uint32_t) (~RCC_CFGR_SW); 									/* (2)  Select HSI as system clock */
				while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI){} 	/* (3)  Wait for HSI switched */
			}
			RCC->CFGR |= RCC_CFGR_PLLSRC_HSI_DIV2;											/*!!! < HSI clock divided by 2 selected as PLL entry clock source */
			RCC->CR &= (uint32_t)(~RCC_CR_PLLON);												/* (4)  Disable the PLL */	
			while((RCC->CR & RCC_CR_PLLRDY) != 0) {}										/* (5)  Wait until PLLRDY is cleared */
			RCC->CFGR |= RCC_CFGR_PLLMUL6;															/* (6)  Set the PLL multiplier to  */
			RCC->CR |= RCC_CR_PLLON; 																		/* (7)  Enable the PLL */
			while((RCC->CR & RCC_CR_PLLRDY) == 0){} 										/* (8)  Wait until PLLRDY is set */
			RCC->CFGR |= (uint32_t) (RCC_CFGR_SW_PLL);									/* (9)  Select PLL as system clock */
			while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL){} 		/* (10) Wait until the PLL is switched on */	
}

void PLLHSEBYPConfig(void)
{
			RCC->CR |= RCC_CR_HSEBYP;
			RCC->CR |= RCC_CR_HSEON;
			//while((RCC->CR & RCC_CR_HSERDY)==0){}
			RCC->CFGR |= RCC_CFGR_PLLSRC_HSE_PREDIV;									/*!< HSE/PREDIV clock selected as PLL entry clock source */
			RCC->CR &= (uint32_t)(~RCC_CR_PLLON);		
			while((RCC->CR & RCC_CR_PLLRDY) != 0){}										/* wait while turning off */
//		RCC->CFGR |= RCC_CFGR_PLLMUL2; 														/* x6 */
			RCC->CFGR = RCC->CFGR & (~RCC_CFGR_PLLMUL) | (RCC_CFGR_PLLMUL6);
			RCC->CR |= RCC_CR_PLLON; 																	/* turn on */
			while((RCC->CR & RCC_CR_PLLRDY) == 0){} 									/* wait while turning on */
			RCC->CFGR |= (uint32_t) (RCC_CFGR_SW_PLL); 								/*!< PLL selected as system clock */
//	  RCC->CFGR |= (uint32_t) (RCC_CFGR_SW_HSE);
		while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL){} 	/*!< PLL used as system clock */
//			while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSE){}
					
}


uint8_t Trig(uint8_t flag)
{
	if(flag) flag = 0; else flag = 1;
	return flag;
}

void DefineYellow(void)
{
	if(flag_yellow_LED==1) SETLEDyellow(); else RESETLEDyellow();
}

void LEDConfig(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	GPIOC->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7 | GPIO_MODER_MODER8 | GPIO_MODER_MODER9);
	GPIOC->MODER |= (GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0);
	GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7 | GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9);
	GPIOC->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7 | GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9);
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR7 | GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9);
}

