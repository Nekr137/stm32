#include "stm32f0xx.h" //xx for family
//LCD + resist



#define PortAClocking()				(RCC->AHBENR |= RCC_AHBENR_GPIOAEN)
#define PortCClocking()				(RCC->AHBENR |= RCC_AHBENR_GPIOCEN)

#define ADCClocking()					(RCC->APB2ENR |= RCC_APB2ENR_ADCEN)


//MY LED macros
#define SETLEDred()						(GPIOC->BSRR = GPIO_BSRR_BS_6)
#define SETLEDgreen()					(GPIOC->BSRR = GPIO_BSRR_BS_9)
#define SETLEDblue()					(GPIOC->BSRR = GPIO_BSRR_BS_7)
#define SETLEDyellow()				(GPIOC->BSRR = GPIO_BSRR_BS_8)
#define RESETLEDred()					(GPIOC->BSRR = GPIO_BSRR_BR_6)
#define RESETLEDgreen()				(GPIOC->BSRR = GPIO_BSRR_BR_9)
#define RESETLEDblue()				(GPIOC->BSRR = GPIO_BSRR_BR_7)
#define RESETLEDyellow()			(GPIOC->BSRR = GPIO_BSRR_BR_8)

#define BUTTON_IN()						(GPIOA->IDR & GPIO_IDR_0)

#define On 1;
#define Off 0;
#define Up 1;
#define Down 0;








//MACROS
#define butt_in()						(GPIOA -> IDR & GPIO_IDR_0)
#define LED_6_ON()						(GPIOC -> BSRR = GPIO_BSRR_BS_6)
#define LED_6_OFF()						(GPIOC -> BSRR = GPIO_BSRR_BR_6)

#define LED_7_ON()						(GPIOC -> BSRR = GPIO_BSRR_BS_7)
#define LED_7_OFF()						(GPIOC -> BSRR = GPIO_BSRR_BR_7)

#define LED_8_ON()						(GPIOC -> BSRR = GPIO_BSRR_BS_8)
#define LED_8_OFF()						(GPIOC -> BSRR = GPIO_BSRR_BR_8)

#define LED_9_ON()						(GPIOC -> BSRR = GPIO_BSRR_BS_9)
#define LED_9_OFF()						(GPIOC -> BSRR = GPIO_BSRR_BR_9)

#define PA15_ON()							(GPIOA -> BSRR = GPIO_BSRR_BS_15)
#define PA15_OFF()						(GPIOA -> BSRR = GPIO_BSRR_BR_15)
#define PC12_ON()							(GPIOC -> BSRR = GPIO_BSRR_BS_12)
#define PC12_OFF()						(GPIOC -> BSRR = GPIO_BSRR_BR_12)
#define PA45_in()             (GPIOA -> IDR & (GPIO_IDR_4 | GPIO_IDR_5 ))

#define Port_A_ON()						(RCC -> AHBENR |= RCC_AHBENR_GPIOAEN)
#define Port_C_ON()						(RCC -> AHBENR |= RCC_AHBENR_GPIOCEN)

#define reset_rs()                      (GPIOB -> BSRR = GPIO_BSRR_BR_12)
#define set_rs()                        (GPIOB -> BSRR = GPIO_BSRR_BS_12)

#define reset_rw()                      (GPIOB -> BSRR = GPIO_BSRR_BR_3)
#define set_rw()                        (GPIOB -> BSRR = GPIO_BSRR_BS_3)

#define reset_en()                      (GPIOC -> BSRR = GPIO_BSRR_BR_4)
#define set_en()                        (GPIOC -> BSRR = GPIO_BSRR_BS_4)

int i = 0;
uint32_t msTick = 0;
uint16_t sec = 0;
	
	
void ShowNum16(uint16_t chislo,uint8_t x, uint8_t y);
void MyClocking(void);
void MyLEDConfigurationRegisters(void);
	 
void SysTick_Handler()
{
	msTick++;
}

	 
	 
	 
void Half_Byte(uint8_t j) 
{
	j = j&0x0F;//po maske clear garbige
	GPIOB -> BSRR = GPIO_BSRR_BR_4 |GPIO_BSRR_BR_5|GPIO_BSRR_BR_6|GPIO_BSRR_BR_7; //clear from garbige	
	GPIOB -> BSRR = j<<4;
}

void LCD_delay(uint32_t j) {
    
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

void LCD_Wait(void) {
    
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

void LCD_WriteCMD(uint8_t j) {
	LCD_Wait();
	reset_rs();
	Half_Byte(j>>4);
	LCD_Pulse();
	Half_Byte(j);
	LCD_Pulse();
}

void LCD_Write_data(uint8_t j) {
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
	ADCClocking();
    
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

void LCD_WriteSTR(uint8_t *str) {
    
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

int main (void) 
{
		volatile int data;//r
	
		SysTick_Config(SystemCoreClock/1000);// 1000HZ TICK
		MyClocking();//r
		MyLEDConfigurationRegisters();//r
		ADCClocking();//r
	
	
		GPIOA->MODER |= GPIO_MODER_MODER1;//r
	

	
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
	

    
    LCD_Config();
    
    LCD_WriteCMD(0x80);
    LCD_delay(10000);

    
    //LCD_WriteSTR("STR1AbCD");
    
    LCD_WriteCMD(0xC0);
    //LCD_WriteSTR("1");
    
    
    //LCD_SetStartPos (12,1);
    //LCD_Write_data ('X');
    
    LCD_WriteCMD(0x01);
    while(1)
		{
							if(ADC1->ISR & ADC_ISR_EOC)
							{
								data = ADC1->DR;//reed from register
								//ADC1->ISR |= ADC_ISR_EOC;//write "1" (interesting moment)
							}
				
			if(msTick>1000)
			{
				msTick = 0;
				ShowNum16(sec,1,0);
				ShowNum16(data,1,1);
				sec++;
			}
		}			
}


void ShowNum16(uint16_t chislo,uint8_t x, uint8_t y)
{
	uint16_t N[5];
	uint32_t indx = 0;
	int temp; 
	
	for(temp = 0;temp<5; temp++)
	{
		N[indx] = chislo % 10;
		chislo = chislo/10;
		indx++;
	}
	
	LCD_SetStartPos(x,y);
	LCD_Write_data(N[4]+'0');
	LCD_Write_data(N[3]+'0');
	LCD_Write_data(N[2]+'0');
	LCD_Write_data(N[1]+'0');
	LCD_Write_data(N[0]+'0');
}

void MyClocking(void)
{
		//RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
		PortAClocking();
	  //RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
		PortCClocking();	
}

void MyLEDConfigurationRegisters(void)
{
	//GPIO port mode register
			GPIOC->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7 | GPIO_MODER_MODER8 | GPIO_MODER_MODER9);
			GPIOC->MODER |= (GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0);
	
	//GPIO port output type register (GPIOx_OTYPER)
			GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7 | GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9);
					
	//GPIO port output speed register (GPIOx_OSPEEDR) (x = A..F)
			GPIOC->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7 | GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9);
					
	//GPIO port pull-up/pull-down register (GPIOx_PUPDR) (x = A..F)
			GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR7 | GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9);
}//END OF void MyLEDConfigurationRegisters();



