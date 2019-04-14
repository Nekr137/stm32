

#include "MyGyr.h"
#include "MySPI.h"

double* Gyr_Callibration(double *V_gyr)
{
	uint16_t receive[18];
		
	receive[6] = 0;//xl callibration
	receive[7] = 0;//xh callibration
	receive[8] = 0;//yl callibration
	receive[9] = 0;//yh callibration
	receive[10] = 0;//zl callibration
	receive[11] = 0;//zh callibration

	for(int indx2 = 0; indx2<100; indx2++)
	{
			receive[0] = ReadGyroscopeAngle(0xE8);//xl
			receive[1] = ReadGyroscopeAngleH(0xE9);//xh
			receive[2] = ReadGyroscopeAngle(0xEA);//yl
			receive[3] = ReadGyroscopeAngleH(0xEB);//yh
			receive[4] = ReadGyroscopeAngle(0xEC);//zl
			receive[5] = ReadGyroscopeAngleH(0xED);//zh		
		
			for(uint16_t indx = 0; indx<6; indx++)
			{
				if(receive[indx] > 32768)
				{
					V_gyr[indx] = 65536 - receive[indx];
					V_gyr[indx] *= -1;
				}
				else
				{
					V_gyr[indx] = receive[indx];
				}
			}
			
			for(uint16_t indx = 0; indx<6; indx++)
			{
				V_gyr[indx+6]+=V_gyr[indx];
			}
	}
	
	for(uint16_t indx = 0; indx<6; indx++)
	{
		V_gyr[indx+6]/=100;
	}
	return V_gyr;
}

double* GyrRefreshData(double *GyrData)
{
	
	int indx = 0;
	uint16_t receive[12];
	
	receive[0] = ReadGyroscopeAngle(0xE8);//xl
	receive[1] = ReadGyroscopeAngleH(0xE9);//xh
	receive[2] = ReadGyroscopeAngle(0xEA);//yl
	receive[3] = ReadGyroscopeAngleH(0xEB);//yh
	receive[4] = ReadGyroscopeAngle(0xEC);//zl
	receive[5] = ReadGyroscopeAngleH(0xED);//zh
	
	for(indx = 0; indx<6; indx++)
	{
		if(receive[indx] > 32768)
		{
			GyrData[indx] = 65536 - receive[indx];
			GyrData[indx] *= -1;
		}
		else
		{
			GyrData[indx] = receive[indx];
		}
		
		if((GyrData[indx]<10)&&(GyrData[indx]>-10))
		{
			GyrData[indx] = 0;
		}
		else
		{
			GyrData[indx]-=GyrData[indx+6];
			GyrData[indx] = GyrData[indx]*0.095*0.02;
		}
		GyrData[indx+12] += GyrData[indx];
	}
	return GyrData;	
}


uint8_t ReadGyroscopeData(uint8_t Address) // reed gyroscop
{
	uint8_t GyroscopeData;
	resetCS_G();
	SPI_SentByte_8(Address);
	GyroscopeData = SPI_SentByte_8(0xFF); //0xFF - garbige
	setCS_G();
 return GyroscopeData;	
}



int8_t ReadGyroscopeData_int8_t(uint8_t Address)
{
	int8_t GyroscopeData;
	resetCS_G();
	SPI_SentByte_8(Address);
	GyroscopeData = SPI_SentByte_8(0xFF); //0xFF - garbige
	setCS_G();
 return GyroscopeData;		
}

void WriteGyroscopeData(uint8_t Address,uint8_t DataToWrite)
{
	resetCS_G();
  SPI_SentByte_8(Address);
	SPI_SentByte_8(DataToWrite);
	setCS_G();
}


uint16_t ReadGyroscopeAngle(uint8_t Address)
{
	uint16_t result;
	uint8_t temp1;
	uint8_t temp2;
	
	resetCS_G();
	SPI_SentByte_8(Address);
	temp1 = SPI_SentByte_8(0x00); //0x00 - garbige
	temp2 = SPI_SentByte_8(0x00);
	setCS_G();
	
	result = ((temp2<<8)|(temp1));
	
	return(result);
}

uint16_t ReadGyroscopeAngleH(uint8_t Address)
{
	uint16_t result;
	uint8_t temp1;
	uint8_t temp2;
	
	resetCS_G();
	SPI_SentByte_8(Address);
	temp1 = SPI_SentByte_8(0x00); //0x00 - garbige
	temp2 = SPI_SentByte_8(0x00);
	setCS_G();
	
	result = ((temp2)|(temp1<<8));
	
	return(result);
}




uint16_t CheckZeroGyr(uint16_t data, uint16_t level)
{
	uint16_t temp = data;
	if((data & 0x8000)==0)//+
	{
		if(data<level) 
		{
			data = 0;
		}
	}
	else//-
	{
		data &= 0x7FFF;
		data = 0x7FFF - data+1;
		if(data<level)
		{
			data = 0;
		}
		else
		{
			data = temp;
		}
	}
	return data;
}
