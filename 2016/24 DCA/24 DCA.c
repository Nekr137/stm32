#include "stm32f0xx.h"                  // Device header
#include "math.h"


uint32_t msTick;
float msTick_DividedBy10;

void DACconfig(void);
void DACsin(float points,uint16_t time_in_ms, uint16_t lines);

float MySIN(float x);

void SysTick_Handler()
{
	msTick+=1;
	msTick_DividedBy10+=1;
}

int main(void)
{	
	SysTick_Config(SystemCoreClock/10000);
	DACconfig();




	while(1)
	{
		DACsin(300,1,100);//time = 300*1	
	}
	
}


void DACconfig(void)
{
	//PA4 pin
	RCC -> AHBENR |= RCC_AHBENR_GPIOAEN;//not necessary
	RCC -> APB1ENR |= RCC_APB1ENR_DACEN;

	GPIOA -> MODER &= ~(GPIO_MODER_MODER4);
	GPIOA -> MODER &= (GPIO_MODER_MODER4);//analog mode, but it can be any, it still works
	GPIOA -> PUPDR &= GPIO_PUPDR_PUPDR4_0;//not necessary
	GPIOA -> OSPEEDR &= GPIO_OSPEEDER_OSPEEDR4;
	
	//GPIOA -> MODER &= ~(GPIO_MODER_MODER0);
	
	DAC->CR |= DAC_CR_EN1;
}

void DACsin(float points,uint16_t time_in_ms, uint16_t lines)
{
	#define pi 3.141592653589793238462643383

	int indx = 0;
	int indx2 = 0;
	//float x[points];
	float *x = (float *)malloc(points*sizeof(float *));
	int k;

	for(indx = 0; indx<points; indx++)
	{
		x[indx] = 2*pi/points*indx;
	}
	indx = 0;
	
	for(indx2 = 0; indx2<lines; indx2++)
	{
		while(indx<points)
		{
			if(msTick_DividedBy10>time_in_ms)
			{
				msTick_DividedBy10 = 0;
				DAC->DHR12R1 = sin(x[indx])*1500+2100;//data holdiing register. 
				indx++;
			}
		}
		indx = 0;
	}	
}



