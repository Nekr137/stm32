#include "stm32f0xx.h"                  // Device header


//for LCD
#define reset_rs()                      (GPIOB -> BSRR = GPIO_BSRR_BR_12)
#define set_rs()                        (GPIOB -> BSRR = GPIO_BSRR_BS_12)

#define reset_rw()                      (GPIOB -> BSRR = GPIO_BSRR_BR_3)
#define set_rw()                        (GPIOB -> BSRR = GPIO_BSRR_BS_3)

#define reset_en()                      (GPIOC -> BSRR = GPIO_BSRR_BR_4)
#define set_en()                        (GPIOC -> BSRR = GPIO_BSRR_BS_4)


//LED
#define SETLEDred()						(GPIOC->BSRR = GPIO_BSRR_BS_6)
#define SETLEDgreen()					(GPIOC->BSRR = GPIO_BSRR_BS_9)
#define SETLEDblue()					(GPIOC->BSRR = GPIO_BSRR_BS_7)
#define SETLEDyellow()				(GPIOC->BSRR = GPIO_BSRR_BS_8)
#define RESETLEDred()					(GPIOC->BSRR = GPIO_BSRR_BR_6)
#define RESETLEDgreen()				(GPIOC->BSRR = GPIO_BSRR_BR_9)
#define RESETLEDblue()				(GPIOC->BSRR = GPIO_BSRR_BR_7)
#define RESETLEDyellow()			(GPIOC->BSRR = GPIO_BSRR_BR_8)


//ECHO
#define EchoUp()						(GPIOC->BSRR = GPIO_BSRR_BS_4)
#define EchoDown()					(GPIOC->BSRR = GPIO_BSRR_BR_4)


//SPI
#define SET_LE()						(GPIOA->BSRR = GPIO_BSRR_BS_8)
#define RESET_LE()					(GPIOA->BSRR = GPIO_BSRR_BR_8)


volatile uint32_t msTick;
volatile uint32_t EchoTick = 0;

int MovePointsFlag = 0;
int SecondsFlag = 1;
int TickTemp = 0;
void LEDconfig(void);


//LCD
void Half_Byte(uint8_t j);
void LCDinit(void);
void LCD_delay(uint16_t j);
void LCD_Pulse(void);
void LCD_Wait(void);
void LCD_WriteCMD(uint8_t j);
void LCD_Write_data(uint8_t j);
void LCD_Config(void);
void LCD_WriteSTR(uint8_t *str);
void LCD_SetStartPos(uint8_t x, uint8_t y);
void LCD_ShowNum2(uint16_t chislo,uint8_t x, uint8_t y);
void LCD_ShowNum16_MinusPlus(uint16_t chislo,uint8_t x, uint8_t y);
void LCD_Show_double_(double t,uint8_t x, uint8_t y);
void LCD_Show_double(double t,uint8_t x, uint8_t y);
void LCD_Rectangle(uint32_t x, uint32_t y);
void LCD_Battery(uint32_t level,uint32_t x, uint32_t y, uint8_t address);
void LCD_Show_uint8(uint8_t t,uint8_t x, uint8_t y);


//SPI
#define SPI2_DR8 (*(__IO uint8_t *)(uint32_t)(SPI2_BASE+0x0C))
uint8_t SPI_SentByte_8(uint8_t byte);		//for gyr
void SPI_SentByte_16(uint16_t data);		//for matrix
void SPIconfig(void);

//OSC
uint16_t OSCMyPoint(uint16_t x, uint16_t y);
int Degree2(int deg);
uint16_t OSCConversionVoltage2Matrix(uint16_t V_ADC);
uint16_t OSCMovePoints(uint16_t v);
void OSCWrite(uint16_t data);

//ECHO
void EchoTrig(void);
void EchoConfig(void);
uint32_t EchoGet(void);


void SysTick_Handler()
{
	msTick++;
	EchoTick ++;
}

int main(void)
{
	uint32_t t = 0;
	SysTick_Config(SystemCoreClock/1000);//1000HzTick
	SPIconfig();	
	LEDconfig();
	EchoConfig();
	LCD_Config();
	
	//LCD_Show_double(t,1,1);
	msTick = 990;
	while(1)
	{
		if(msTick>100)
		{
			msTick = 0;
			t = EchoGet();
			LCD_Show_uint8(t,1,1);
		}
	}
}


uint32_t EchoGet(void)
{
	uint32_t time;

	//EchoUp();
	SETLEDgreen();
	for(volatile int indx = 0; indx<5; indx++);	
	RESETLEDgreen();

	
	
	while(!(GPIOC->IDR & GPIO_IDR_8))//0
	{
	}	
	time = EchoTick; 
	//SETLEDred();
	while((GPIOC->IDR & GPIO_IDR_8))
	{
	}
	
	return EchoTick-time;
}


void EchoConfig(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	
	GPIOA->MODER &= ~(GPIO_MODER_MODER4|GPIO_MODER_MODER5);
	GPIOA->MODER |= (GPIO_MODER_MODER4_0);
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_4 | GPIO_OTYPER_OT_5); 
	GPIOA->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR4 | GPIO_OSPEEDER_OSPEEDR5);
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR4 | GPIO_PUPDR_PUPDR5);
}


void LEDconfig(void)
{
			RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	
			GPIOC->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7 | GPIO_MODER_MODER8 | GPIO_MODER_MODER9);
			GPIOC->MODER |= (GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0| GPIO_MODER_MODER9_0);
			GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7 | GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9);
			GPIOC->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7 | GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9);
			GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR7 | GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9);
}




void OSCWrite(uint16_t data)
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



uint8_t SPI_SentByte_8(uint8_t byte)
{
	while((SPI2->SR & SPI_SR_BSY)==SPI_SR_BSY){}
	SPI2_DR8 = byte;
	while((SPI2->SR & SPI_SR_BSY)==SPI_SR_BSY){}
	return(SPI2_DR8);
}

void SPI_SentByte_16(uint16_t data)		//for matrix
{
			while((SPI2->SR & SPI_SR_BSY)==SPI_SR_BSY){}
			SPI2->DR = data;
			while((SPI2->SR & SPI_SR_BSY) == SPI_SR_BSY){}
}


void SPIconfig(void)
{
  RCC -> AHBENR |= RCC_AHBENR_GPIOAEN;
  RCC -> AHBENR |= RCC_AHBENR_GPIOBEN;
	RCC -> AHBENR |= RCC_AHBENR_GPIOCEN;
  RCC -> APB1ENR |= RCC_APB1ENR_SPI2EN;
	
	GPIOA->MODER &= ~(GPIO_MODER_MODER8); //LE GENERAL OUTPUT
	GPIOA->MODER |= (GPIO_MODER_MODER8_0);
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_8);//LE PushPull
  GPIOA->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR8);
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR8);//LE PUPD
	
	GPIOB->MODER &= ~(GPIO_MODER_MODER13|GPIO_MODER_MODER15|GPIO_MODER_MODER14);//MOSI,SLC ALTERNATIVE FUNC
	GPIOB->MODER |= (GPIO_MODER_MODER13_1|GPIO_MODER_MODER15_1|GPIO_MODER_MODER14_1);
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT_13|GPIO_OTYPER_OT_15);//MOSI,SLC PushPull
	GPIOB->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR13|GPIO_OSPEEDER_OSPEEDR15);	
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR13|GPIO_PUPDR_PUPDR15|GPIO_PUPDR_PUPDR14);//MOSI,SLC PUPD
	
	GPIOB->AFR[1] &= ~( GPIO_AFRH_AFRH5 | GPIO_AFRH_AFRH7);
	
	GPIOC->MODER &= ~(GPIO_MODER_MODER0);
	GPIOC->MODER |= (GPIO_MODER_MODER0_0);
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR0);

	SPI2->CR1 = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_BR | SPI_CR1_MSTR | SPI_CR1_CPOL | SPI_CR1_CPHA;
  //SPI2->CR2 = SPI_CR2_DS;//osc - data size
	//SPI2->CR2 = (0x07<<8)|(SPI_CR2_FRXTH);//gyr  /*!< FIFO reception Threshold */
  SPI2->CR1 |= SPI_CR1_SPE;									// spi peripheral enable
	
	while((SPI2->SR & SPI_SR_BSY)==SPI_SR_BSY){}
}





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


void LCD_Show_uint8(uint8_t t,uint8_t x, uint8_t y)
{
	uint16_t N[6];


	LCD_SetStartPos(x,y);
	
	for(int indx = 0;indx<6; indx++)
	{
		N[indx] = t % 10;
		t /= 10;
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



