// Tree Timer + Video 19/12/16
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

//for LCD
#define reset_rs()                      (GPIOB -> BSRR = GPIO_BSRR_BR_12)
#define set_rs()                        (GPIOB -> BSRR = GPIO_BSRR_BS_12)

#define reset_rw()                      (GPIOB -> BSRR = GPIO_BSRR_BR_3)
#define set_rw()                        (GPIOB -> BSRR = GPIO_BSRR_BS_3)

#define reset_en()                      (GPIOC -> BSRR = GPIO_BSRR_BR_4)
#define set_en()                        (GPIOC -> BSRR = GPIO_BSRR_BS_4)
//end "for LCD"


uint32_t msTick = 0;
uint32_t sTick = 0;
uint32_t mTick = 0;
uint32_t hTick = 0;
uint32_t dTick = 0;
uint32_t MyButtonState = 0;
uint32_t MyButtonCount = 0;
uint32_t Set2ZeroFlag = 0;
uint32_t EndOfCountFlag = 0;





void MyTimer(void);
void MyModeSelect(void);
void MyResetAllLEDs(void);

//lcd fn's
void MyLEDConfigurationRegisters(void);
void Half_Byte(uint8_t j);
void LCD_delay(uint32_t j);
void LCD_Pulse(void);
void LCD_Wait(void);
void LCD_WriteCMD(uint8_t j);
void LCD_Write_data(uint8_t j);
void LCD_Config(void);
void LCD_WriteSTR(uint8_t *str);
void LCD_SetStartPos(uint8_t x, uint8_t y);
void ShowNum16(uint16_t chislo,uint8_t x, uint8_t y);
void ShowMyClock16(uint8_t x, uint8_t y);

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
	volatile int LCD_data;
	SysTick_Config(SystemCoreClock/1000);//1000HzTick
	MyLEDConfigurationRegisters();
	
	//LCD
	LCD_Config();  
  LCD_WriteCMD(0x80);
  LCD_delay(10000);
	
	LCD_WriteCMD(0xC0);
		//LCD_WriteSTR("STR1AbCD");
		//LCD_WriteSTR("1");  
		//LCD_SetStartPos (12,1);
		//LCD_Write_data ('X');
	LCD_WriteCMD(0x01);
	//END LCD
	
	
	
	while(1)
	{
		
		//LCD
				if(ADC1->ISR & ADC_ISR_EOC)
				{
					LCD_data = ADC1->DR;//reed from register
				}
				//ShowNum16(sec,1,0);
				LCD_data = sTick;
				//ShowNum16(LCD_data,1,1);
				ShowMyClock16(1,1);
		//END LCD
		
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
	if(msTick>=1000)
	{
		msTick = msTick-1000;// I think value can not be reset because it can be greater then 1000
		sTick++;
	}
	if(sTick>=60)
	{
		sTick = 0;
		mTick++;
	}
	if(mTick>=60)
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



void Half_Byte(uint8_t j) 
{
	j = j&0x0F;//po maske clear garbige
	GPIOB -> BSRR = GPIO_BSRR_BR_4 |GPIO_BSRR_BR_5|GPIO_BSRR_BR_6|GPIO_BSRR_BR_7; //clear from garbige	
	GPIOB -> BSRR = j<<4;
}

void LCD_delay(uint32_t j)
{
	while(j--){}
}

void LCD_Pulse(void) 
{
uint32_t i;  
	for(i =0; i<10; i++){}    
	set_en();
	for(i =0; i<10; i++){}
	reset_en();
	for(i =0; i<10; i++){}
}

void LCD_Wait(void) 
{
    
    uint8_t j = 0;
    
    GPIOB -> MODER &= ~ GPIO_MODER_MODER4;//config port for reading
    GPIOB -> MODER &= ~ GPIO_MODER_MODER5;
    GPIOB -> MODER &= ~ GPIO_MODER_MODER6;
    GPIOB -> MODER &= ~ GPIO_MODER_MODER7;
    
    reset_rs();//comand
		set_rw();	// read

     while(1) 
		{
         set_en();
         LCD_delay(5);
         
         j = (GPIOB->IDR & GPIO_IDR_7);        
				reset_en();
         
				LCD_Pulse();
        if(j==0) break;
         
	}
    //port for write obratno  
    GPIOB->MODER |= (GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0);
    reset_rw();
}

void LCD_WriteCMD(uint8_t j) 
{
	LCD_Wait();
	reset_rs();
	Half_Byte(j>>4);
	LCD_Pulse();
	Half_Byte(j);
	LCD_Pulse();
}

void LCD_Write_data(uint8_t j) 
{
	LCD_Wait();
	set_rs();
	Half_Byte(j>>4);
	LCD_Pulse();
	Half_Byte(j);
	LCD_Pulse();
}

void LCD_Config(void) 
{
    
    RCC -> AHBENR |= RCC_AHBENR_GPIOBEN;
    RCC -> AHBENR |= RCC_AHBENR_GPIOCEN;
		RCC->APB2ENR |= RCC_APB2ENR_ADCEN;
    
    GPIOB -> MODER &= ~ (GPIO_MODER_MODER3 | GPIO_MODER_MODER4 | GPIO_MODER_MODER5 | GPIO_MODER_MODER6 | GPIO_MODER_MODER7 | GPIO_MODER_MODER12);
    GPIOB -> MODER |= GPIO_MODER_MODER3_0 |GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER12_0; //01
    GPIOB -> PUPDR |= (GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0);    
    
    GPIOC->MODER &= ~(GPIO_MODER_MODER4 | GPIO_MODER_MODER6| GPIO_MODER_MODER8);
		GPIOC->MODER |= GPIO_MODER_MODER4_0;
		GPIOC->MODER |= GPIO_MODER_MODER6_0;
		GPIOC->MODER |= GPIO_MODER_MODER8_0;
		
		GPIOC->BSRR = GPIO_BSRR_BS_6; //light
    
  reset_rs();
	reset_rw();
	reset_en();	
	LCD_delay(10000);
    
	Half_Byte(0x02);
	LCD_Pulse();    
	Half_Byte(0x02);
	LCD_Pulse();
  Half_Byte(0x02);
	LCD_Pulse();    
	Half_Byte(0x08);//initial position of carret
	LCD_Pulse();
    
	LCD_delay(50000);
    
	LCD_WriteCMD(0x28);//for displey
	LCD_Pulse();
    
	LCD_WriteCMD(0x0C);//for display
	LCD_Pulse();
    
	LCD_WriteCMD(0x06);//=""'
	LCD_Pulse();
    
	LCD_WriteCMD(0x01);
	LCD_Pulse();
  LCD_delay(50000);
}

void LCD_WriteSTR(uint8_t *str)
{
    
    uint8_t a;
    
    while (1) 
		{
        a = *str;
        
        if(a) 
				{
            LCD_Write_data (a);
        }
        else break;      
        str++;//move to the next     
    }        
}

void LCD_SetStartPos(uint8_t x, uint8_t y)
{
	
    x = x & 0x0F;
    x = 0x80 + (x-1);
    
    if(y) 
		{
        x = x | 0x40; //dobavit 1 eto smeshenie na stroku
    }
    
    LCD_WriteCMD(x);
}

void ShowNum16(uint16_t chislo,uint8_t x, uint8_t y)
{
	uint16_t N[9];
	uint32_t indx = 0;
	int temp; 
	
	for(temp = 0;temp<8; temp++)
	{
		N[indx] = chislo % 10;
		chislo = chislo/10;
		indx++;
	}
	
	LCD_SetStartPos(x,y);

	LCD_Write_data(N[8]+'0');
	LCD_Write_data(N[7]+'0');
	LCD_Write_data(':');
	LCD_Write_data(N[6]+'0');
	LCD_Write_data(N[5]+'0');
	LCD_Write_data(':');
	LCD_Write_data(N[4]+'0');
	LCD_Write_data(N[3]+'0');
	LCD_Write_data(':');
	LCD_Write_data(N[2]+'0');
	LCD_Write_data(N[1]+'0');
	LCD_Write_data(N[0]+'0');
}

void ShowMyClock16(uint8_t x, uint8_t y)
{
	uint16_t N[11];
	uint32_t indx = 0;
	uint32_t msTick_ = 0;
	uint32_t sTick_ = 0;
	uint32_t mTick_ = 0;
	uint32_t hTick_ = 0;
	uint32_t dTick_ = 0;	
	
	int temp; 
	
	msTick_ = msTick;	sTick_ = sTick; mTick_ = mTick; hTick_ = hTick; dTick_ = dTick;
	
	for(temp = 0;temp<3; temp++)
	{
		N[indx] = msTick_ % 10;
		msTick_ = msTick_/10;
		indx++;
	}
	for(temp = 0;temp<2; temp++)
	{
		N[indx] = sTick_ % 10;
		sTick_ = sTick/10;
		indx++;
	}
	for(temp = 0;temp<2; temp++)
	{
		N[indx] = mTick_ % 10;
		mTick_ = mTick_/10;
		indx++;
	}
	for(temp = 0;temp<2; temp++)
	{
		N[indx] = hTick_ % 10;
		hTick_= hTick_/10;
		indx++;
	}
	for(temp = 0;temp<2; temp++)
	{
		N[indx] = dTick_ % 10;
		dTick_ = dTick_/10;
		indx++;
	}
	
	LCD_SetStartPos(x,y);

	LCD_Write_data(N[10]+'0');
	LCD_Write_data(N[9]+'0');	
	LCD_Write_data(' ');
	LCD_Write_data(N[8]+'0');
	LCD_Write_data(N[7]+'0');
	LCD_Write_data(':');
	LCD_Write_data(N[6]+'0');
	LCD_Write_data(N[5]+'0');
	LCD_Write_data(':');
	LCD_Write_data(N[4]+'0');
	LCD_Write_data(N[3]+'0');
	LCD_Write_data(':');
	LCD_Write_data(N[2]+'0');
	LCD_Write_data(N[1]+'0');
	LCD_Write_data(N[0]+'0');
}
