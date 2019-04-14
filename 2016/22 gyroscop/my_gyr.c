//gyroscop

//try to read the ctrl_reg_1 and get the OD and BW, PD, Z,X,Y enable
//																									PO - 0 - low energy consumption, so PO must be 1
//																									default - 0000 0111, try - 0000 1111
//																									so WriteGyroscopeData(0x20,0x0F);

//			stat = ReadGyroscopeData(STATUS_gyr);//  0000 1111
//			ctrl1 = ReadGyroscopeData(CTRL1_gyr);// 0000 changing if msTick = 



//lsb - least significant bit
//2^16 = 65536
//2^15 = 32768
//try to minus

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


//spi8
#define SPI2_DR8 (*(__IO uint8_t *)(uint32_t)(SPI2_BASE+0x0C))


//for LCD
#define reset_rs()                      (GPIOB -> BSRR = GPIO_BSRR_BR_12)
#define set_rs()                        (GPIOB -> BSRR = GPIO_BSRR_BS_12)

#define reset_rw()                      (GPIOB -> BSRR = GPIO_BSRR_BR_3)
#define set_rw()                        (GPIOB -> BSRR = GPIO_BSRR_BS_3)

#define reset_en()                      (GPIOC -> BSRR = GPIO_BSRR_BR_4)
#define set_en()                        (GPIOC -> BSRR = GPIO_BSRR_BS_4)

#define resetCS_G()											(GPIOC -> BSRR = GPIO_BSRR_BR_0)
#define setCS_G()						 						(GPIOC -> BSRR = GPIO_BSRR_BS_0)

uint32_t msTick;
uint32_t msTick20;
float myTick;

//lcd fn's
void MyLEDConfigurationRegisters(void);
void LCDinit(void);
void Half_Byte(uint8_t j);
void LCD_delay(uint32_t j);
void LCD_Pulse(void);
void LCD_Wait(void);
void LCD_WriteCMD(uint8_t j);
void LCD_Write_data(uint8_t j);
void LCD_Config(void);
void LCD_WriteSTR(uint8_t *str);
void LCD_SetStartPos(uint8_t x, uint8_t y);
void LCD_ShowNum2(uint16_t chislo,uint8_t x, uint8_t y);
void LCD_ShowNum16(uint16_t chislo,uint8_t x, uint8_t y);
void LCD_ShowNum16_MinusPlus(uint16_t chislo,uint8_t x, uint8_t y);
void LCD_Show_double_(double t,uint8_t x, uint8_t y);
void LCD_Show_double(double t,uint8_t x, uint8_t y);
void LCD_Rectangle(uint32_t x, uint32_t y);
void LCD_Battery(uint32_t level,uint32_t x, uint32_t y, uint8_t address);

//gyroscop
void SPI_Config(void);
uint8_t ReadGyroscopeData(uint8_t Address);
void WriteGyroscopeData(uint8_t Address,uint8_t DataToWrite);
uint16_t ReadGyroscopeAngle(uint8_t Address);
uint16_t ReadGyroscopeAngleH(uint8_t Address);
uint16_t CheckZeroGyr(uint16_t data, uint16_t level);
int8_t ReadGyroscopeData_int8_t(uint8_t Address);
void LCD_Show_Gyr(uint16_t data,uint8_t x, uint8_t y);
double* Gyr_Pos(double *V_gyr);
double* Gyr_Callibration(double *V_gyr);
uint8_t SPI_SentByte_8(uint8_t byte);
//spi
//uint8_t SPIget8(uint8_t Adress);
//void SPIsent8(uint8_t Adress);


void SysTick_Handler()
{
	myTick+=0.1;
}

int main(void)
{
	double V_gyr[12];//xl xh; yl yh; zl zh;
	double P_gyr0[12];
	uint8_t level = 0;

	
	SysTick_Config(SystemCoreClock/10000);//1000HzTick

	MyLEDConfigurationRegisters();
  LCDinit();
	
	SPI_Config();

	WriteGyroscopeData(0x20,0x0F);//CTRL_REG1 - start L3GD20 - 95Hz,12.5 cutt-off
	WriteGyroscopeData(0x23,0x00);//sensitivity dps - degree per second, default 0000 0000, 250 dps, 4wire
	
	//LCD_SetStartPos(2,1);	LCD_WriteSTR("Callibration"); LCD_SetStartPos(2,2); LCD_WriteSTR("gyroscope");
	//Gyr_Callibration(V_gyr);
	
	
	

//		LCD_WriteCMD(0x01);
//		LCD_WriteCMD(0x0F);

//		LCD_WriteCMD(0x2C);
//		//LCD_Rectangle(5,2);	
//		LCD_Battery(4,16,1,0x00);
//		LCD_Battery(4,1,1,0x03);



		while(1)
		{
			LCD_Show_Gyr(V_gyr[0],1,1);
		}
}



double* Gyr_Callibration(double *V_gyr)
{
int indx = 0;
int indx2 = 0;
int Flag_msTick = 0;
	uint16_t receive[12];
		
	receive[6] = 0;//xl callibration
	
	receive[7] = 0;//xh callibration
	receive[8] = 0;//yl callibration
	receive[9] = 0;//yh callibration
	receive[10] = 0;//zl callibration
	receive[11] = 0;//zh callibration

	for(indx2 = 0; indx2<100; indx2++)
	{
		if(msTick20>20)
		{
			msTick20 = 0;
			Flag_msTick = 1;
		}
		else
		{
			indx2--;
		}
		if(Flag_msTick == 1)
		{
			Flag_msTick = 0;
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
					V_gyr[indx] = 65536 - receive[indx];
					V_gyr[indx] *= -1;
				}
				else
				{
					V_gyr[indx] = receive[indx];
				}
			}
			for(indx = 0; indx<6; indx++)
			{
				V_gyr[indx+6]+=V_gyr[indx];
			}
		}
	}
	for(indx = 0; indx<6; indx++)
	{
		V_gyr[indx+6]/=100;
	}
}

double* Gyr_Pos(double *V_gyr)
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
			V_gyr[indx] = 65536 - receive[indx];
			V_gyr[indx] *= -1;
		}
		else
		{
			V_gyr[indx] = receive[indx];
		}
		
		if((V_gyr[indx]<1)&&(V_gyr[indx]>-1))
		{
			V_gyr[indx] = 0;
		}
		else
		{
			V_gyr[indx]-=V_gyr[indx+6];// *= 0.095*0.01;
			V_gyr[indx] = V_gyr[indx]*360/32768;
		}
	}
	return V_gyr;	
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

uint8_t SPI_SentByte_8(uint8_t byte)
{
	while((SPI2->SR & SPI_SR_BSY)==SPI_SR_BSY){}
	SPI2_DR8 = byte;
	while((SPI2->SR & SPI_SR_BSY)==SPI_SR_BSY){}
	return(SPI2_DR8);
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

// unnecessary function. 

//uint8_t SPIget8(uint8_t Address)//get inf from SPI (8bit)
//{
//	while((SPI2->SR & SPI_SR_BSY)==SPI_SR_BSY){}
//	SPI2_DR8 = Address;
//	while((SPI2->SR & SPI_SR_BSY)==SPI_SR_BSY){}
//	return(SPI2_DR8);
//}


void SPI_Config(void)
{
  RCC -> AHBENR |= RCC_AHBENR_GPIOAEN;
  RCC -> AHBENR |= RCC_AHBENR_GPIOBEN;
	RCC -> AHBENR |= RCC_AHBENR_GPIOCEN;
  RCC -> APB1ENR |= RCC_APB1ENR_SPI2EN;
	
	GPIOA->MODER &= ~(GPIO_MODER_MODER8); //LE GENERAL OUTPUT
	GPIOA->MODER |= (GPIO_MODER_MODER8_0);
    
	GPIOB->MODER &= ~(GPIO_MODER_MODER13|GPIO_MODER_MODER15|GPIO_MODER_MODER14);//MOSI,SLC ALTERNATIVE FUNC
	GPIOB->MODER |= (GPIO_MODER_MODER13_1|GPIO_MODER_MODER15_1|GPIO_MODER_MODER14_1);
    
    //type of alt func
    //GPIOB->AFR[1] |= AFn << (pin_N-8)*4; //-8 if we want to write starshii
    //GPIOB->AFR[] &= ~( GPIO_AFRH_AFRH5 | GPIO_AFRH_AFRH7); //7 sootvetstvuet15    
	
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_8);//LE PushPull
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT_13|GPIO_OTYPER_OT_15);//MOSI,SLC PushPull
	
	GPIOA->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR8);
	GPIOB->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR13|GPIO_OSPEEDER_OSPEEDR15);	
	
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR8);//LE PUPD
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR13|GPIO_PUPDR_PUPDR15|GPIO_PUPDR_PUPDR14);//MOSI,SLC PUPD

	GPIOC->MODER &= ~(GPIO_MODER_MODER0);
	GPIOC->MODER |= (GPIO_MODER_MODER0_0);
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR0);

	SPI2->CR1 = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_BR | SPI_CR1_MSTR;// | SPI_CR1_CPOL | SPI_CR1_CPHA;
  //SPI2->CR2 = SPI_CR2_DS;
	SPI2->CR2 = (0x07<<8)|(SPI_CR2_FRXTH);
  SPI2->CR1 |= SPI_CR1_SPE;	
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
  

void LCD_Show_Gyr(uint16_t data,uint8_t x, uint8_t y)
{
	uint16_t N[16];
	uint32_t indx = 0;
	int temp; 
	

	
	if(data>=32768)
	{
		data = 32768-data;
	}
	else
	{
		data += 0x80;
	}

	for(temp = 0;temp<6; temp++)
	{
		N[indx] = data % 10;
		data /= 10;
		indx++;
	}	
	
	LCD_SetStartPos(x,y);
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
	int frac = 0;
	
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

