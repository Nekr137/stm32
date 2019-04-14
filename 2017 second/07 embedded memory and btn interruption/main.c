/* Nekr 02-11-2017 */

/* project where I studied to use EXTinterruptions and tryed to save variable into the embedded flash memory. */

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

#define FlashLock()						(FLASH->CR |= FLASH_CR_LOCK)
#define adr1()								(0x0800F000)

uint8_t btn_trg = 0;							  /* button trigger */
uint8_t flag_change_regime = 0;			

void EXTIConfig(void);
void LEDConfig(void);
void FlashUnlock(void);
uint8_t FlashReady(void);
void FlashErasePage(uint32_t page);
void FlashWrite(uint32_t page, uint32_t data);
uint32_t FlashRead(uint32_t page);



void SetRegime(void)
{
	ResetAllLEDs(); 
	switch(btn_trg)
	{
		case 0:	SETLEDred(); break;	
		case 1:	SETLEDyellow(); break; 
	  case 2: SETLEDblue(); break; 
	  case 3: SETLEDgreen(); break;
	}
}

void EXTI0_1_IRQHandler(void)
{
	flag_change_regime = 1;
	EXTI->PR |= EXTI_PR_PR0;			
}

int main(void)
{
	LEDConfig();	EXTIConfig();	FlashUnlock();

	btn_trg = FlashRead(adr1()); SetRegime();
	
	while(1)
	{
		if(flag_change_regime==1) 
		{
			flag_change_regime = 0;
			btn_trg++; if(btn_trg==4) btn_trg=0;
		  SetRegime();
			FlashWrite(adr1(),btn_trg); 
		}
	}
}


void EXTIConfig(void)
{
		RCC->AHBENR |= RCC_AHBENR_GPIOAEN; 		/* (1) Enable the peripheral clock of GPIOA */
		SYSCFG->EXTICR[1] &= (uint16_t)~SYSCFG_EXTICR1_EXTI0_PA; /* (2) Select Port A for pin 0 external interrupt by writing 0000 in EXTI0 (reset value)*/ 
		GPIOA->MODER &= ~GPIO_MODER_MODER0; /* input mode */
		GPIOA->PUPDR |= ~GPIO_PUPDR_PUPDR0; /* no pull-up no pull-down (floating input) */ 
	
  	EXTI->IMR = EXTI_IMR_IM0;       /* (3) Configure the corresponding mask bit in the EXTI_IMR register */
		EXTI->RTSR = EXTI_RTSR_RT0;			/* (4) Configure the Trigger Selection bits of the Interrupt line on rising edge*/
		//EXTI->FTSR = EXTI_FTSR_FT0; 		/* (5) Interrupt line on falling edge (push-up the button)*/
		
		/* Configure NVIC for External Interrupt */
		NVIC_EnableIRQ(EXTI0_1_IRQn); 				/* (1) Enable Interrupt on EXTI0_1 */
		NVIC_SetPriority(EXTI0_1_IRQn,0); 		/* (2)	 Set priority for EXTI0_1 */
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

uint32_t FlashRead(uint32_t page)
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

