//MATRIX 02.12.2016 (one point in the left-bottom corner)


//SCK - PB13
//MOSI - PB15
//LE - PA8 (set as GPIO)


#include "stm32f0xx.h"                  // Device header

#define SET_LE()						(GPIOA->BSRR = GPIO_BSRR_BS_8)
#define RESET_LE()					(GPIOA->BSRR = GPIO_BSRR_BR_8)


void Config(void);
uint16_t MyPoint(uint16_t x, uint16_t y);
int Degree2 (int deg);

int main(void)
{
	uint16_t data;
	
	//configuration
	Config();
	data = MyPoint(8,8);//point

	while((SPI2->SR & SPI_SR_BSY)==SPI_SR_BSY)
	{
	}

	RESET_LE();
	
	SPI2->DR = data;
	
	while((SPI2->SR & SPI_SR_BSY) == SPI_SR_BSY)
	{
	}
	
	SET_LE();

	while(1)
	{
	}
}

uint16_t MyPoint(uint16_t x, uint16_t y)
{
	uint16_t data;
	data = Degree2(x-1)+256*Degree2(y-1);
}

int Degree2 (int deg) 
{
		int Degree2_out = 1;
		while (deg) {
			Degree2_out *= 2;
			deg--;
		}
		return Degree2_out;
}


void Config(void)
{
//initialization for matrix

  //clocking
  RCC -> AHBENR |= RCC_AHBENR_GPIOAEN;
  RCC -> AHBENR |= RCC_AHBENR_GPIOBEN;
  RCC -> APB1ENR |= RCC_APB1ENR_SPI2EN; //joke!
	
	GPIOA->MODER &= ~(GPIO_MODER_MODER8); //LE GENERAL OUTPUT
	GPIOA->MODER |= (GPIO_MODER_MODER8_0);
    
	GPIOB->MODER &= ~(GPIO_MODER_MODER13|GPIO_MODER_MODER15);//MOSI,SLC ALTERNATIVE FUNC
	GPIOB->MODER |= (GPIO_MODER_MODER13_1|GPIO_MODER_MODER15_1);
    
  //type of alt func
  //GPIOB->AFR[1] |= AFn << (pin_N-8)*4; //-8 if we want to write starshii
  GPIOB->AFR[1] &= ~( GPIO_AFRH_AFRH5 | GPIO_AFRH_AFRH7); //7 sootvetstvuet15  
		 
	
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_8);//LE PushPull
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT_13|GPIO_OTYPER_OT_15);//MOSI,SLC PushPull
	
	GPIOA->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR8);
	GPIOB->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR13|GPIO_OSPEEDER_OSPEEDR15);	
	
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR8);//LE PUPD
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR13|GPIO_PUPDR_PUPDR15);//MOSI,SLC PUPD
	
	SPI2->CR1 = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_BR | SPI_CR1_SSM | SPI_CR1_MSTR | SPI_CR1_CPOL | SPI_CR1_CPHA;
  SPI2->CR2 = SPI_CR2_DS;
  SPI2->CR1 |= SPI_CR1_SPE;
}


