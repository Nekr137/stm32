
#include "MyOsc.h"
#include "MySPI.h"


void OSCWriteADC(uint16_t data)
{
			RESET_LE();
			SPI_SentByte_16(data);
			SET_LE();	
}

uint16_t OSCConversionVoltage2Matrix(uint16_t V_ADC)
{
	uint16_t v;
	if(V_ADC<10)
	{
		v = 10;
	}
	if((V_ADC<=512) && (V_ADC>10))
	{
		v = 0;
	}
	if((V_ADC>512) && (V_ADC<=1024))
	{
		v = 1;
	}
	if((V_ADC>1024) && (V_ADC<=1536))
	{
		v = 2;
	}
	if((V_ADC>1536) && (V_ADC<=2048))
	{
		v = 3;
	}
	if((V_ADC>2048) && (V_ADC<=2560))
	{
		v = 4;
	}
	if((V_ADC>2560) && (V_ADC<= 3072))
	{
		v = 5;
	}
	if((V_ADC>3072) && (V_ADC <= 3584))
	{ 
		v = 6;
	}
	if(V_ADC>3584)
	{
		v = 7;
	}
	return(v);
}

uint16_t OSCMyPoint(uint16_t x, uint16_t y)
{
	uint16_t data = 0;
	data = Degree2(x)+256*Degree2(y);
	return(data);
}


int Degree2(int deg) 
{
		int Degree2_out = 1;
		while (deg) {
			Degree2_out *= 2;
			deg--;
		}
		return Degree2_out;
}


