

#include "MyDAC.h"
#include "math.h"


void DACwrite(uint16_t value)
{
	//#define Delay 2
	//for(uint32_t i = 0; i<Delay; i++){}
	DAC->DHR12R1 = value;//data holdiing register. 
}


void DACconfig(void)
{
	//PA4 pin
	RCC -> AHBENR |= RCC_AHBENR_GPIOAEN;				//not necessary
	RCC -> APB1ENR |= RCC_APB1ENR_DACEN;

	GPIOA -> MODER &= ~(GPIO_MODER_MODER4);
	GPIOA -> MODER |= (GPIO_MODER_MODER4);			//analog mode, but it can be any, it still works
	GPIOA -> PUPDR |= GPIO_PUPDR_PUPDR4_0;			//not necessary
	GPIOA -> OSPEEDR |= GPIO_OSPEEDER_OSPEEDR4; //high speed

	
	
	DAC->DHR8R1 |= DAC_DHR8R1_DACC1DHR;					//8bit, right aligned
	DAC->CR |= DAC_CR_BOFF1;
	//DAC->CR |= DAC_CR_WAVE1;										//triangle wave generation enabled
	//DAC->CR |= DAC_CR_MAMP1_2;
	DAC->CR |= DAC_CR_TSEL1;
	
	
	
	DAC->CR |= DAC_CR_EN1;
}


uint16_t* My64sin(uint16_t * my_sin)
{

	uint16_t y[256] = {2047,2097,2148,2198,2248,2299,2349,2398,2448,2497,2546,2595,2643,2692,2739,2786,2833,2880,2925,2971,3015,3060,3103,3146,3188,3230,3271,3311,3350,3388,3426,3463,3499,3534,3568,3602,3634,3665,3696,3725,3753,3781,3807,3832,3856,3879,3901,3922,3942,3960,3978,3994,4009,4023,4035,4047,4057,4066,4073,4080,4085,4089,4092,4094,4094,4093,4091,4087,4083,4077,4070,4061,4052,4041,4029,4016,4001,3986,3969,3951,3932,3912,3891,3868,3844,3820,3794,3767,3739,3710,3681,3650,3618,3585,3551,3517,3481,3445,3407,3369,3330,3291,3250,3209,3167,3125,3081,3038,2993,2948,2903,2856,2810,2763,2715,2668,2619,2571,2522,2473,2423,2373,2324,2274,2223,2173,2123,2072,2022,1971,1921,1871,1820,1770,1721,1671,1621,1572,1523,1475,1426,1379,1331,1284,1238,1191,1146,1101,1056,1013,969,927,885,844,803,764,725,687,649,613,577,543,509,476,444,413,384,355,327,300,274,250,226,203,182,162,143,125,108,93,78,65,53,42,33,24,17,11,7,3,1,0,0,2,5,9,14,21,28,37,47,59,71,85,100,116,134,152,172,193,215,238,262,287,313,341,369,398,429,460,492,526,560,595,631,668,706,744,783,823,864,906,948,991,1034,1079,1123,1169,1214,1261,1308,1355,1402,1451,1499,1548,1597,1646,1696,1745,1795,1846,1896,1946,1997,2047};
	
	for(int indx = 0; indx<256; indx++)
	{
		my_sin[indx] = y[indx];
	}
	return my_sin;
}


void DACsin(float points,uint32_t time_in_ms, uint16_t lines)
{
	#define pi 3.141592653589793238462643383
	#define Delay 10000

	int indx = 0;
	int indx2 = 0;
	//float x[points];
	float *x = (float *)malloc(points*sizeof(float *));

	for(indx = 0; indx<points; indx++)
	{
		x[indx] = 2*pi/points*indx;
	}
	indx = 0;
	
	for(indx2 = 0; indx2<lines; indx2++)
	{
		while(indx<points)
		{
//			if(msTick>time_in_ms)
//			{
//				msTick = 0;
				for(uint32_t i = 0; i<Delay; i++){}
				DAC->DHR12R1 = sin(x[indx])*1500+2100;//data holdiing register. 
				indx++;
//			}
		}
		indx = 0;
	}	
}


