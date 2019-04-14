//gyroscop

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
//end "for LCD"

#define resetCS_G()											(GPIOC -> BSRR = GPIO_BSRR_BR_0)
#define setCS_G()						 						(GPIOC -> BSRR = GPIO_BSRR_BS_0)


uint32_t msTick;

//lcd fn's
void MyLEDConfigurationRegisters(void);
void LCD_Start_inmain(void);
void Half_Byte(uint8_t j);
void LCD_delay(uint32_t j);
void LCD_Pulse(void);
void LCD_Wait(void);
void LCD_WriteCMD(uint8_t j);
void LCD_Write_data(uint8_t j);
void LCD_Config(void);
void LCD_WriteSTR(uint8_t *str);
void LCD_SetStartPos(uint8_t x, uint8_t y);
void LCD_ShowNum16(uint16_t chislo,uint8_t x, uint8_t y);
//void ShowMyClock16(uint8_t x, uint8_t y);

//gyroscop
void SPI_Config(void);
uint8_t GReadRegister(uint8_t Adress);
void SPIWriteRegister(uint8_t Adress,uint8_t data);

//spi
uint8_t SPIget8(uint8_t Adress);
void SPIsent8(uint8_t Adress);

void SysTick_Handler()
{
	msTick++;
}

int main(void)
{
	SysTick_Config(SystemCoreClock/1000);//1000HzTick
	MyLEDConfigurationRegisters();
  LCD_Start_inmain();
	
	SPI_Config();
	
	//SPIWriteRegister(0x0F,0xFF);
	 
	while(1)
	{
		if(msTick%1000>900)
		{
			//0F - who am I, but "1" at the beginning of the byte means "reed from gyroscop". So 10001111= 0x8F. 
			LCD_ShowNum16(GReadRegister(0x8F),1,1);
		}
	}
}

uint8_t GReadRegister(uint8_t Adress) // reed gyroscop
{
	uint8_t data;
	resetCS_G();
	SPIget8(Adress);
	data = SPIget8(0xFF);
	setCS_G();
 return data;	
}

uint8_t SPIget8(uint8_t Adress)//get inf from SPI (8bit)
{
	while((SPI2->SR & SPI_SR_BSY)==SPI_SR_BSY){}
	SPI2_DR8 = Adress;
	while((SPI2->SR & SPI_SR_BSY)==SPI_SR_BSY){}
	return(SPI2_DR8);
}


void GWriteRegister(uint8_t Adress,uint8_t data)
{
	resetCS_G();
  SPIsent8(Adress);
	setCS_G();
}

void SPIsent8(uint8_t Adress)
{
	while((SPI2->SR & SPI_SR_BSY)==SPI_SR_BSY){}
	SPI2_DR8 = 0xFF;
	while((SPI2->SR & SPI_SR_BSY)==SPI_SR_BSY){}
}

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

void LCD_Start_inmain(void)
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
    x = x & 0x0F;
    x = 0x80 + (x-1);
    if(y) 
		{
        x = x | 0x40; //dobavit 1 eto smeshenie na stroku
    }
    LCD_WriteCMD(x);
}

void LCD_ShowNum16(uint16_t chislo,uint8_t x, uint8_t y)
{
	uint16_t N[6];
	uint32_t indx = 0;
	int temp; 
	
	for(temp = 0;temp<6; temp++)
	{
		N[indx] = chislo % 10;
		chislo = chislo/10;
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
