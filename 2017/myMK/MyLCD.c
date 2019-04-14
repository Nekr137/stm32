

#include "MyLCD.h"




void LCDinit(void)
{
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
}


void Half_Byte(uint8_t j) 
{
	j = j&0x0F;//po maske clear garbige
	GPIOB -> BSRR = GPIO_BSRR_BR_4 |GPIO_BSRR_BR_5|GPIO_BSRR_BR_6|GPIO_BSRR_BR_7; //clear from garbige	
	GPIOB -> BSRR = j<<4;
}

void LCD_delay(uint16_t j)
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

void LCD_Config(void) {
    
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
		
		GPIOC->BSRR |= GPIO_BSRR_BS_6; //light
    
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
		y--;
    x = 0x80 + (x-1);
    if(y) x = x | 0x40;
    LCD_WriteCMD(x);
}

void LCD_MySetStartPos(uint8_t x, uint8_t y)
{
	
}

void LCD_ShowNum2(uint16_t chislo,uint8_t x, uint8_t y)
{
	uint16_t N[16];
	uint32_t indx = 0;
	int temp; 
	
	for(temp = 0;temp<16; temp++)
	{
		N[indx] = chislo % 2;
		chislo = chislo/2;
		indx++;
	}
	
	LCD_SetStartPos(x,y);
	LCD_Write_data(N[15]+'0');
	LCD_Write_data(N[14]+'0');	
	LCD_Write_data(N[13]+'0');
	LCD_Write_data(N[12]+'0');
	LCD_Write_data(N[11]+'0');
	LCD_Write_data(N[10]+'0');
	LCD_Write_data(N[9]+'0');
	LCD_Write_data(N[8]+'0');
	LCD_Write_data(N[7]+'0');
	LCD_Write_data(N[6]+'0');	
	LCD_Write_data(N[5]+'0');
	LCD_Write_data(N[4]+'0');
	LCD_Write_data(N[3]+'0');
	LCD_Write_data(N[2]+'0');
	LCD_Write_data(N[1]+'0');
	LCD_Write_data(N[0]+'0');
}
  
void LCD_Show_double_(double t,uint8_t x, uint8_t y)
{
	uint16_t N[8];
	int indx = 0;
	int frac = 0;
	int whole = 0;
	uint16_t d = 0;
	
	whole = (int) t; 
	frac = (t - whole)*100;
	

	d = frac;
	for(indx = 0;indx<3; indx++)
	{
		N[indx] = d % 10;
		d = d/10;
	}
	d = whole;
	
	LCD_SetStartPos(x,y);
	if(d>32768)
	{
		LCD_Write_data('-');
		d = 65536 - d;
	}
	else
	{
		LCD_Write_data('+');
	}

	for(indx = 3; indx<8; indx++)
	{
		N[indx] = d % 10;
		d = d/10;		
	}
	
	LCD_SetStartPos(x+1,y);
	LCD_Write_data(N[7]+'0');
  LCD_Write_data(N[6]+'0');	
	LCD_Write_data(N[5]+'0');
	LCD_Write_data(N[4]+'0');
	LCD_Write_data(N[3]+'0');
	LCD_Write_data('.');
	LCD_Write_data(N[1]+'0');
	LCD_Write_data(N[0]+'0');
}


void LCD_Show_double(double t,uint8_t x, uint8_t y)
{
	uint16_t N[6];
	uint16_t d = 0;
	int indx = 0;
	int whole = 0;
	
	whole = (int) t; 


	LCD_SetStartPos(x,y);
	if(whole<0)
	{
		LCD_Write_data('-');
		whole *= -1;
	}
	else
	{
		LCD_Write_data('+');
	}

	d = whole;
	
	for(indx = 0;indx<6; indx++)
	{
		N[indx] = d % 10;
		d /= 10;
	}	
	
	LCD_SetStartPos(x+1,y);
	LCD_Write_data(N[5]+'0');
	LCD_Write_data(N[4]+'0');
	LCD_Write_data(N[3]+'0');
	LCD_Write_data(N[2]+'0');
	LCD_Write_data(N[1]+'0');
	LCD_Write_data(N[0]+'0');
}

void LCD_Rectangle(uint32_t x, uint32_t y)
{
		//rectangle
		LCD_WriteCMD(0x40);
		LCD_Write_data(0x1F);
		LCD_Write_data(0x11);
		LCD_Write_data(0x11);
		LCD_Write_data(0x11);
		LCD_Write_data(0x11);
		LCD_Write_data(0x11);
		LCD_Write_data(0x11);
		LCD_Write_data(0x1F);
		LCD_WriteCMD(0x01);
		LCD_SetStartPos(x,y);
		LCD_Write_data(0x00);
}

void LCD_Battery(uint32_t level,uint32_t x, uint32_t y, uint8_t address)
{
	uint8_t indx = 0;
	level = 5-level;
		LCD_WriteCMD(0x40 | address<<3);
			LCD_Write_data(0x04);
			LCD_Write_data(0x1F);
			for(indx = 0; indx<level; indx++) LCD_Write_data(0x11);
			for(indx=level; indx<5; indx++) LCD_Write_data(0x1F);
			LCD_Write_data(0x1F);		
		LCD_WriteCMD(0x01);
		LCD_SetStartPos(x,y);
		LCD_Write_data(address);
}


