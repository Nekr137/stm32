#include "stm32f0xx.h"  


#define reset_rs() GPIOB->BSRR = GPIO_BSRR_BR_12;
#define set_rs() GPIOB->BSRR = GPIO_BSRR_BS_12;
#define reset_rw() GPIOB->BSRR = GPIO_BSRR_BR_3;
#define set_rw() GPIOB->BSRR = GPIO_BSRR_BS_3;
#define reset_en() GPIOC->BSRR = GPIO_BSRR_BR_4;
#define set_en() GPIOC->BSRR = GPIO_BSRR_BS_4;

#define yel_on() (GPIOC->BSRR = GPIO_BSRR_BS_8)
#define yel_off() (GPIOC->BSRR = GPIO_BSRR_BR_8)
//uint16_t j;
/*
#define blue_on() (GPIOC->BSRR = GPIO_BSRR_BS_7)
#define blue_off() (GPIOC->BSRR = GPIO_BSRR_BR_7)

#define red_on() (GPIOC->BSRR = GPIO_BSRR_BS_6)
#define red_off() (GPIOC->BSRR = GPIO_BSRR_BR_6)

#define green_on() (GPIOC->BSRR = GPIO_BSRR_BS_9)
#define green_off() (GPIOC->BSRR = GPIO_BSRR_BR_9)
*\







*\
int RED_CNT=30000;
int RED_YEL_CNT=10000;
int YEL_CNT=10000;
int GREEN_CNT=30000;
int GREEN_BLK_CNT=30000;
int flag = 0;

uint16_t j;

int user_cnt = 30;

int flag1=0;

int TL_CNT=0;

int TICK_CNT;

int flag_sec;

int flag_msec;

void SysTick_Handler()
{
	
	j = GPIOA->IDR & 1;
	if (j == 1)
	{
		user_cnt++;
	}
	else 
	{
		user_cnt=0;
	}
	if (user_cnt == 30)
	{

		TL_CNT=1;
		flag=0;
		RED_CNT=30000;
		RED_YEL_CNT=10000;
		YEL_CNT=10000;
		GREEN_CNT=30000;
		GREEN_BLK_CNT=30000;
	}
	
	
	
	if(++TICK_CNT == 1000)
	{
		flag_msec = TICK_CNT;
		TICK_CNT=0;
		flag_sec =1;

	}
	
}
*/
void Half_Byte(uint16_t j)
{
	j = j&0x0F;
	//clear legs
	GPIOB->BSRR = GPIO_BSRR_BR_4 |GPIO_BSRR_BR_5|GPIO_BSRR_BR_6|GPIO_BSRR_BR_7;
	
	GPIOB->BSRR = j<<4;
}

void LCD_Pulse(void)
{
	for(int i =0; i<10; i++){}
	set_en();
	for(int i =0; i<10; i++){}
	reset_en();
	for(int i =0; i<10; i++){}
}

void LCD_Wait(void)
{
	uint16_t j = 0;
	GPIOB->MODER &= ~(GPIO_MODER_MODER4 | GPIO_MODER_MODER5 | GPIO_MODER_MODER6 | GPIO_MODER_MODER7);
	
	reset_rs();
	set_rw();
	
	while(1)
	{
		set_en();
		j = (GPIOB->IDR & GPIO_IDR_7);
		reset_en();
		LCD_Pulse();
		if(!j) break;
	}
	
	GPIOB->MODER |= (GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0);
	reset_rw();
	
}

void LCD_Write_data(uint16_t j)
{
	LCD_Wait();
	set_rs();
	Half_Byte(j>>4);
	LCD_Pulse();
	Half_Byte(j);
	LCD_Pulse();
}

void LCD_delay(uint32_t j)
{
	while(j--){}
}

void LCD_WriteCMD(uint16_t j)
{
	LCD_Wait();
	reset_rs();
	Half_Byte(j>>4);
	LCD_Pulse();
	Half_Byte(j);
	LCD_Pulse();
}
void LCD_WriteStr(char *s)
{
	while(*s)
	{
		LCD_Write_data(*s++);
	}
}
void LCD_Config()
{
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	
	GPIOB->MODER &= ~(GPIO_MODER_MODER3 |GPIO_MODER_MODER4 | GPIO_MODER_MODER5 | GPIO_MODER_MODER6 | GPIO_MODER_MODER7 | GPIO_MODER_MODER12);
	GPIOB->MODER |= GPIO_MODER_MODER3_0 |GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER12_0;
	
	GPIOB->PUPDR |= (GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0);
	
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	
	GPIOC->MODER &= ~(GPIO_MODER_MODER4 | GPIO_MODER_MODER6| GPIO_MODER_MODER8);
	GPIOC->MODER |= GPIO_MODER_MODER4_0;
	GPIOC->MODER |= GPIO_MODER_MODER6_0;
	GPIOC->MODER |= GPIO_MODER_MODER8_0;
		
	GPIOC->BSRR = GPIO_BSRR_BS_6;
		
	//4bit
	reset_rs();
	reset_rw();
	reset_en();
	
	LCD_delay(10000);
	Half_Byte(0x02);
	LCD_Pulse();
	Half_Byte(0x02);
	LCD_Pulse();
	Half_Byte(0x08);
	LCD_Pulse();
	LCD_delay(10000);
	LCD_WriteCMD(0x28);
	LCD_Pulse();
	LCD_WriteCMD(0x0C);
	LCD_Pulse();
	LCD_WriteCMD(0x06);
	LCD_Pulse();
	LCD_WriteCMD(0x01);
	LCD_Pulse();
	yel_on();
}







int main(){
	
	LCD_Config();

	LCD_WriteCMD(0x80);
	LCD_WriteStr("Dobrochat");
	while(1){};
}



