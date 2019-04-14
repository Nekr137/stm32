
#include <stm32f0xx.h>

//"define" for the matrix



#define SET_LE()						(GPIOA->BSRR = GPIO_BSRR_BS_8)
#define RESET_LE()					(GPIOA->BSRR = GPIO_BSRR_BR_8)
//

//void OSCconfig(void);
uint16_t OSCMyPoint(uint16_t x, uint16_t y);
int Degree2(int deg);
uint16_t OSCConversionVoltage2Matrix(uint16_t V_ADC);
uint16_t OSCMovePoints(uint16_t v);
void OSCWriteADC(uint16_t data);


//int main(void)
//{
//	uint16_t data = 0;
//	volatile int ADC_data;//r
//	int V_ADC = 0;
//	uint16_t voltage[8];//vertical
//	int k = 0;
//	int k1 = 0;
//	
//  
//	SysTick_Config(SystemCoreClock/1000);//1000HzTick

//	//configuration
//	Config();
//	GPIOA->MODER |= GPIO_MODER_MODER1;//set "1" to the resist
//						
//  Start_ADC_inmain();

//	while((SPI2->SR & SPI_SR_BSY)==SPI_SR_BSY)
//	{
//	}
//	
//	while(1)
//	{
//		if(MovePointsFlag==1)
//		{
//			MovePointsFlag = 0;
//			for(k1 = 0; k1<=4; k1++)
//			{
//				voltage[k1] = voltage[k1+1];
//			}
//			voltage[5] = voltage[6];
//			voltage[6] = voltage[7];
//			voltage[7] = voltage[0];
//		}
//		
//		//MeasureVoltage
//		if(ADC1->ISR & ADC_ISR_EOC)//r
//		{
//			V_ADC = ADC1->DR;//reed from register
//			//ADC1->ISR |= ADC_ISR_EOC;//write "1" (interesting moment)
//		}
//		voltage[4] = ConversionVoltage2Matrix(V_ADC);
//		
//		
//		for(k = 0; k<8; k++)
//		{
//			data = MyPoint(k,voltage[k]);
//			WriteADC(data);
//		}
//	}
//}
