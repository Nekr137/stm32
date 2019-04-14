//osc

#include "stm32f0xx.h"                  // Device header

//"define" for the matrix
#define SET_LE()						(GPIOA->BSRR = GPIO_BSRR_BS_8)
#define RESET_LE()					(GPIOA->BSRR = GPIO_BSRR_BR_8)
//


uint32_t msTick;
int MovePointsFlag = 0;
int SecondsFlag = 1;
int TickTemp = 0;


void Config(void);
uint16_t MyPoint(uint16_t x, uint16_t y);
int Degree2(int deg);
int ConversionVoltage2Matrix(int V_ADC);
uint16_t MovePoints(uint16_t v);
void WriteADC(uint16_t data);
void Start_ADC_inmain(void);


void SysTick_Handler()
{
	msTick++;
	if(msTick>200)
	{
		msTick = 0;
		MovePointsFlag = 1;
		SecondsFlag = 1; 
	}
}

int main(void)
{
	uint16_t data = 0;
	volatile int ADC_data;//r
	int V_ADC = 0;
	uint16_t voltage[8];//vertical
	int k = 0;
	int k1 = 0;
	
  
	SysTick_Config(SystemCoreClock/1000);//1000HzTick

	//configuration
	Config();
	GPIOA->MODER |= GPIO_MODER_MODER1;//set "1" to the resist
						
  Start_ADC_inmain();

	while((SPI2->SR & SPI_SR_BSY)==SPI_SR_BSY)
	{
	}
	
	while(1)
	{
		if(MovePointsFlag==1)
		{
			MovePointsFlag = 0;
			for(k1 = 0; k1<=4; k1++)
			{
				voltage[k1] = voltage[k1+1];
			}
			voltage[5] = voltage[6];
			voltage[6] = voltage[7];
			voltage[7] = voltage[0];
		}
		
		//MeasureVoltage
		if(ADC1->ISR & ADC_ISR_EOC)//r
		{
			V_ADC = ADC1->DR;//reed from register
			//ADC1->ISR |= ADC_ISR_EOC;//write "1" (interesting moment)
		}
		voltage[4] = ConversionVoltage2Matrix(V_ADC);
		
		
		for(k = 0; k<8; k++)
		{
			data = MyPoint(k,voltage[k]);
			WriteADC(data);
		}
	}
}

void WriteADC(uint16_t data)
{
			RESET_LE();
			SPI2->DR = data;
			while((SPI2->SR & SPI_SR_BSY) == SPI_SR_BSY)
			{
			}
			SET_LE();	
}

int ConversionVoltage2Matrix(int V_ADC)
{
	int v;
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

uint16_t MyPoint(uint16_t x, uint16_t y)
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

void Start_ADC_inmain()
{
	RCC->APB2ENR |= RCC_APB2ENR_ADCEN;//res clocking
	ADC1->CR = ADC_CR_ADCAL;//"=" - all disabled 
	while((ADC1->CR & ADC_CR_ADCAL)!=0)
	{
	}
	ADC1->CR |= ADC_CR_ADEN; //adc begin work contiieosly!
	while(!(ADC1->ISR & ADC_ISR_ADRDY))
	{
	}
	ADC1->CFGR1 |= ADC_CFGR1_CONT;
	//config1 
	ADC1->CHSELR = ADC_CHSELR_CHSEL1;//channel 1
	ADC1->CR |= ADC_CR_ADSTART;	
}

void Config(void)
{
//initialization for matrix

  	//clocking
    RCC -> AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC -> AHBENR |= RCC_AHBENR_GPIOBEN;
    RCC -> APB1ENR |= RCC_APB1ENR_SPI2EN; //joke!
		RCC -> AHBENR |= RCC_AHBENR_GPIOCEN;
	
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
		
		
//below - my initialization, but there is one mistake that we could not find, 
//so I took Vasya's code (above)

//	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
//	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
//	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;//joke
//	
//	//ports to the alternative functions
//	
//	
//	GPIOA->MODER &= ~GPIO_MODER_MODER8;
//	GPIOA->MODER |= GPIO_MODER_MODER8_0; //hand
//	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_8;
//	GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR8;
//	//GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR8;
//	
//	GPIOB->MODER &= ~(GPIO_MODER_MODER13 | GPIO_MODER_MODER15);
//	GPIOB->MODER |= (GPIO_MODER_MODER13_1 | GPIO_MODER_MODER15_1); //alt fn
//	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT_13 | GPIO_OTYPER_OT_15);
//	GPIOB->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR13 | GPIO_OSPEEDER_OSPEEDR15);
//	//GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR13 | GPIO_PUPDR_PUPDR15);

//	//GPIOB->AFR[1] = 2<<(13-8)*4;
//	GPIOB->AFR[1] &= ~(GPIO_AFRH_AFRH5 | GPIO_AFRH_AFRH7);

//	//cr1 register
//	SPI2->CR1 = (SPI_CR1_CPHA | SPI_CR1_CPOL | SPI_CR1_MSTR | SPI_CR1_BR | SPI_CR1_SSM | SPI_CR1_SSI);
//	//cr2 register
//	SPI2->CR2 = (SPI_CR2_DS);
//	
//	SPI2->CR1 = SPI_CR1_SPE;//last	
}


