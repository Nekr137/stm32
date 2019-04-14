/*-------------------------*/
/* --- NEKR 21.10.2017 --- */
/*-------------------------*/



/* CR - control register */
/* SR - status register, BSY - flash operation is in progress */


#include "stm32f0xx.h"                  

#define SETLEDred()						(GPIOC->BSRR = GPIO_BSRR_BS_6)
#define SETLEDgreen()					(GPIOC->BSRR = GPIO_BSRR_BS_9)
#define SETLEDblue()					(GPIOC->BSRR = GPIO_BSRR_BS_7)
#define SETLEDyellow()				(GPIOC->BSRR = GPIO_BSRR_BS_8)

#define RESETLEDred()					(GPIOC->BSRR = GPIO_BSRR_BR_6)
#define RESETLEDgreen()				(GPIOC->BSRR = GPIO_BSRR_BR_9)
#define RESETLEDblue()				(GPIOC->BSRR = GPIO_BSRR_BR_7)
#define RESETLEDyellow()			(GPIOC->BSRR = GPIO_BSRR_BR_8)

#define address()			(0x0800F000)

uint32_t DataTrigger(uint32_t data);
void ButtonConfig(void);
void LEDConfig(void);
void FlashUnlock(void);
void FlashLock(void);
uint8_t FlashReady(void);
void FlashErasePage(uint32_t page);
void FlashWrite(uint32_t page, uint32_t data);
uint32_t FLASHRead(uint32_t page);
	
uint16_t msCount = 0;
uint16_t btn_cnt = 0;
uint16_t flag_btn_on = 0;

void SysTick_Handler()
{
	msCount++;
	if (GPIOA->IDR & GPIO_IDR_0)
	{
			btn_cnt++;
			SETLEDred();
	}
	else
	{		
		if(btn_cnt >=20) 
		{
			flag_btn_on = 1; 
		}
		btn_cnt = 0;
		RESETLEDred();
	}
}

int main(void)
{
	uint16_t data = 0;
	
	SysTick_Config(SystemCoreClock/1000);// 1000HZ TICK
	LEDConfig();
	ButtonConfig();
	
	FlashUnlock();
	data = FLASHRead(address());			/* remember what LED worked before */

	while(1)
	{
		if(flag_btn_on)
		{
			flag_btn_on = 0;	
			data = DataTrigger(data);
			FlashWrite(address(),data);
		}
		if(data==1) {SETLEDblue();RESETLEDyellow();}
		else {SETLEDyellow();RESETLEDblue();}
	}

}




uint32_t DataTrigger(uint32_t data)
{
	if(data==1) data=0;
	else data = 1;
	return data;
}


void ButtonConfig(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER &= ~(GPIO_MODER_MODER0);
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

void FlashUnlock(void) 
{
  FLASH->KEYR = FLASH_KEY1;		/* the keys to unlock the flash */
  FLASH->KEYR = FLASH_KEY2;		
}

void FlashLock() 
{
  FLASH->CR |= FLASH_CR_LOCK;
}

uint8_t FlashReady(void)
{
	return !(FLASH->SR & FLASH_SR_BSY); /* wait while flash operation is in progress */
}

void FlashErasePage(uint32_t page)
{
	while( !FlashReady());				/* check that no main Flash operation is ongoin */
	FLASH->CR |= FLASH_CR_PER; 		/* set bit for erasing one page */ 
	FLASH->AR = page; 						/* set the addredss of erasing page */
	FLASH->CR |= FLASH_CR_STRT; 	/* start page erasing */
	while( !FlashReady()); 				/* wait for page erasing */
	FLASH->CR &= ~FLASH_CR_PER; 	/* reset bit for erasing one page */
}

uint32_t FLASHRead(uint32_t page)
{
    return (*(__IO uint32_t*)page);
}


void FlashWrite(uint32_t page, uint32_t data)
{
	FlashErasePage(page);
	while( !FlashReady());										/* check that no main Flash operation is ongoin */
	FLASH->CR |= FLASH_CR_PG; 								/* allow page programming */
	while( !FlashReady()); 										/* wait */
	*(__IO uint16_t*)page = (uint16_t)data; 	/* write first 2 bits */
  while(!FlashReady());											
  page+=2;
  data>>=16;
  *(__IO uint16_t*)page = (uint16_t)data;   /* write last 2 bits */
  while(!FlashReady());
  FLASH->CR &= ~(FLASH_CR_PG); 							/* disallow page programming */
}




