

#include <stm32f0xx.h>


#define resetCS_G()											(GPIOC -> BSRR = GPIO_BSRR_BR_0)
#define setCS_G()						 					(GPIOC -> BSRR = GPIO_BSRR_BS_0)

//gyroscop

double* Gyr_Callibration(double *V_gyr);
double* GyrRefreshData(double *GyrData);  															
uint8_t ReadGyroscopeData(uint8_t Address);											
void WriteGyroscopeData(uint8_t Address,uint8_t DataToWrite);
uint16_t ReadGyroscopeAngle(uint8_t Address);
uint16_t ReadGyroscopeAngleH(uint8_t Address);
uint16_t CheckZeroGyr(uint16_t data, uint16_t level);
int8_t ReadGyroscopeData_int8_t(uint8_t Address);


//int main()
//{
//	double GyrData[18];//xl,xh,yl,yh,zl,zh,xl0,xh0,yl0,yh0,zl0,zh0,pxl,plh,pyl,pyh,pzl,plh

//	SysTick_Config(SystemCoreClock/1000);

//	LCDinit();
//	SPI_Config();

//	Gyr_Callibration(GyrData);
//	
//	while(1)
//	{				
//		if(msTick20>20)
//		{
//			GyrRefreshData(GyrData);
//			msTick20 = 0;
//		}
//		LCD_Show_double(GyrData[0],1,2);
//		LCD_Show_double(GyrData[12],9,2);
//		LCD_Show_double(GyrData[6],1,1);
//	}
//}


