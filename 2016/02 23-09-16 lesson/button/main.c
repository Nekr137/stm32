
#include "stm32f0xx.h" 

#define BUTTON_IN()	    (GPIOA->IDR & GPIO_IDR_0)	
#define LED6OFF()		(GPIOC->ODR &= ~GPIO_ODR_6)
#define LED6ON()		(GPIOC->ODR |= GPIO_ODR_6)


void MyLEDConfigurationRegisters();
void MyTactirovaniye();
void MyRotate();
void MyDelay();
void OnLEDprer();

uint32_t MyTemp;

void SysTickHandler()//here fly processor every ms
{
	if(MyTemp==0)
	{
		LED6OFF();
	}
	else
	{
		MyTemp=MyTemp-1;
		LED6ON();
	}
}


int main()
{

	SysTick_Config(SystemCoreClock/1000); //8Mz

	MyTemp = 100;

	
	
	
	//tactirovaniye
		MyTactirovaniye();
	
	//configuration registers
		GPIOA->MODER &= ~GPIO_MODER_MODER0;
		MyLEDConfigurationRegisters();
		
	//my program
		//MyRotate();
		OnLEDprer();

	
		
}

void MyTactirovaniye()
{	
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
}

void MyLEDConfigurationRegisters()
{
    /// Configuration registers
    
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



void MyRotate()
{
	uint32_t MyInput;// my new variable
			

	MyInput=0;
	
	MyInput = BUTTON_IN();
	
	while(1)
	{
		if(MyInput==0)
		{
			GPIOC->BSRR = GPIO_BSRR_BS_6;//bs - bit set, br - bit reset
			MyDelay();
			GPIOC->BSRR = GPIO_BSRR_BR_6; 
			MyDelay();
			
			GPIOC->BSRR = GPIO_BSRR_BS_9;//bs - bit set, br - bit reset
			MyDelay();
			GPIOC->BSRR = GPIO_BSRR_BR_9; 
			MyDelay();
			
			GPIOC->BSRR = GPIO_BSRR_BS_7;//bs - bit set, br - bit reset
			MyDelay();
			GPIOC->BSRR = GPIO_BSRR_BR_7; 
			MyDelay();
						 
			GPIOC->BSRR = GPIO_BSRR_BS_8;//bs - bit set, br - bit reset
			MyDelay();
			GPIOC->BSRR = GPIO_BSRR_BR_8; 
			MyDelay();
		}
		else
		{
			GPIOC->BSRR = GPIO_BSRR_BS_6;//bs - bit set, br - bit reset
			MyDelay();
			GPIOC->BSRR = GPIO_BSRR_BR_6; 
			MyDelay();
			
			GPIOC->BSRR = GPIO_BSRR_BS_8;//bs - bit set, br - bit reset
			MyDelay();
			GPIOC->BSRR = GPIO_BSRR_BR_8; 
			MyDelay();
			
			GPIOC->BSRR = GPIO_BSRR_BS_7;//bs - bit set, br - bit reset
			MyDelay();
			GPIOC->BSRR = GPIO_BSRR_BR_7; 
			MyDelay();
						 
			GPIOC->BSRR = GPIO_BSRR_BS_9;//bs - bit set, br - bit reset
			MyDelay();
			GPIOC->BSRR = GPIO_BSRR_BR_9; 
			MyDelay();
		}  
	}
}

void MyDelay()
{
	static uint32_t i;
	for(i=0;i<100000L;i++);
}





void OnLEDprer()
{
		uint32_t MyInput;// my new variable
		MyInput=0;
		
	while(1)
	{
		MyInput = BUTTON_IN();
		
		if(MyInput=1)
		{
			MyTemp = 100;
		}
		
		
		
	}
}





