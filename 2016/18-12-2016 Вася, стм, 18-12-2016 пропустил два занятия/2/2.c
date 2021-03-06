
//16-12-2016 17:21
#include "stm32f0xx.h" //xx for family

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

#define PA15_ON()						(GPIOA -> BSRR = GPIO_BSRR_BS_15)
#define PA15_OFF()						(GPIOA -> BSRR = GPIO_BSRR_BR_15)
#define PC12_ON()						(GPIOC -> BSRR = GPIO_BSRR_BS_12)
#define PC12_OFF()						(GPIOC -> BSRR = GPIO_BSRR_BR_12)
#define PA45_in()                       (GPIOA -> IDR & (GPIO_IDR_4 | GPIO_IDR_5 ))

#define Port_A_ON()						(RCC -> AHBENR |= RCC_AHBENR_GPIOAEN)
#define Port_C_ON()						(RCC -> AHBENR |= RCC_AHBENR_GPIOCEN)

#define reset_rs()                      (GPIOB -> BSRR = GPIO_BSRR_BR_12)
#define set_rs()                        (GPIOB -> BSRR = GPIO_BSRR_BS_12)

#define reset_rw()                      (GPIOB -> BSRR = GPIO_BSRR_BR_3)
#define set_rw()                        (GPIOB -> BSRR = GPIO_BSRR_BS_3)

#define reset_en()                      (GPIOC -> BSRR = GPIO_BSRR_BR_4)
#define set_en()                        (GPIOC -> BSRR = GPIO_BSRR_BS_4)


int k = 0;


uint8_t flag = 0;
volatile uint32_t DATA = 0; //for ACP
volatile uint32_t tmpDATA = 0; //for ACP



   
    void Half_Byte(uint8_t j) {
	j = j&0x0F;//po maske clear garbige
	GPIOB -> BSRR = GPIO_BSRR_BR_4 |GPIO_BSRR_BR_5|GPIO_BSRR_BR_6|GPIO_BSRR_BR_7; //clear from garbige	
	GPIOB -> BSRR = j<<4;
}

void LCD_delay(uint32_t j) {
    
	while(j--){}
}

void LCD_Pulse(void) {
    uint8_t i;    
	for(i =0; i<10; i++){}    
	set_en();
	for(i =0; i<10; i++){}
	reset_en();
	for(i =0; i<10; i++){}
}

void LCD_Wait(void) {
    
    uint8_t j = 0;
    
    GPIOB -> MODER &= ~ GPIO_MODER_MODER4;
    GPIOB -> MODER &= ~ GPIO_MODER_MODER5;
    GPIOB -> MODER &= ~ GPIO_MODER_MODER6;
    GPIOB -> MODER &= ~ GPIO_MODER_MODER7;
    
    reset_rs();
	set_rw();
    
    /*while(j != 0) {
    
		set_en();
		j = (GPIOB->IDR & GPIO_IDR_7);
        
		reset_en();
		LCD_Pulse();
	}
    */
     while(1) {
         set_en();
         LCD_delay(5);
         
         j = (GPIOB->IDR & GPIO_IDR_7);        
		 reset_en();
         
		 LCD_Pulse();
         if(j==0) break;
         
	}
     
    GPIOB -> MODER |= (GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0);
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
    RCC -> AHBENR |= RCC_AHBENR_GPIOAEN; //for ACP
    
    
    GPIOB -> MODER &= ~ (GPIO_MODER_MODER3 | GPIO_MODER_MODER4 | GPIO_MODER_MODER5 | GPIO_MODER_MODER6 | GPIO_MODER_MODER7 | GPIO_MODER_MODER12);
    GPIOB -> MODER |= GPIO_MODER_MODER3_0 |GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER12_0; //01
    GPIOB -> PUPDR |= (GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0);    
    
    GPIOC->MODER &= ~(GPIO_MODER_MODER4 | GPIO_MODER_MODER6| GPIO_MODER_MODER7| GPIO_MODER_MODER8| GPIO_MODER_MODER9);
	GPIOC->MODER |= GPIO_MODER_MODER4_0;
	GPIOC->MODER |= GPIO_MODER_MODER6_0;
	GPIOC->MODER |= GPIO_MODER_MODER8_0;
	GPIOC->MODER |= GPIO_MODER_MODER9_0;
    GPIOC->MODER |= GPIO_MODER_MODER7_0;
    
    //for ACP
    //taktirovanie
	RCC -> APB2ENR |= RCC_APB2ENR_ADCEN;
	
	//calibration
	ADC1 -> CR = ADC_CR_ADCAL;
	while (ADC1 -> CR == ADC_CR_ADCAL) {
	}
	
	//turn on
	ADC1 -> CR = ADC_CR_ADEN;
	
	//ready?
	while (!(ADC1 -> ISR & ADC_ISR_ADRDY)){
	}
	
	//
	ADC1 -> CFGR1 = ADC_CFGR1_CONT;
		
	//
	ADC1 -> CHSELR = ADC_CHSELR_CHSEL1;
	
	//
	ADC1 -> CR |= ADC_CR_ADSTART;
	
	
	
	//out pin
	GPIOC->MODER &= ~ (GPIO_MODER_MODER6 | GPIO_MODER_MODER8 | GPIO_MODER_MODER7 | GPIO_MODER_MODER9| GPIO_MODER_MODER12);
	GPIOC->MODER |= GPIO_MODER_MODER6_0 | GPIO_MODER_MODER8_0| GPIO_MODER_MODER7_0| GPIO_MODER_MODER9_0| GPIO_MODER_MODER12_0  ; //01
    
    GPIOA->MODER &= ~ GPIO_MODER_MODER15;
    GPIOA->MODER |= GPIO_MODER_MODER15_0;
		
	//o type
	GPIOC->OTYPER &= ~ (GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_7 | GPIO_OTYPER_OT_9);	
	
	//speed
	GPIOC->OSPEEDR &= ~ (GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR7 | GPIO_OSPEEDER_OSPEEDR9);		
	
	//pull up pull down
	GPIOC->PUPDR &= ~   (GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR7| GPIO_PUPDR_PUPDR9);
    GPIOA->PUPDR &= ~   (GPIO_PUPDR_PUPDR4 | GPIO_PUPDR_PUPDR5);
    GPIOA->PUPDR |=     (GPIO_PUPDR_PUPDR4_1 | GPIO_PUPDR_PUPDR5_1);
    //for ACP END
    
    
    
    
		
	  GPIOC->BSRR = GPIO_BSRR_BS_6; //light
//    GPIOC->BSRR = GPIO_BSRR_BS_7; //light
//    GPIOC->BSRR = GPIO_BSRR_BS_8; //light
//    GPIOC->BSRR = GPIO_BSRR_BS_9; //light
    
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
	Half_Byte(0x08);
	LCD_Pulse();
    
	LCD_delay(50000);
    
	LCD_WriteCMD(0x28);
	LCD_Pulse();
    
	LCD_WriteCMD(0x0C);
	LCD_Pulse();
    
	LCD_WriteCMD(0x06);
	LCD_Pulse();
    
	LCD_WriteCMD(0x01);
	LCD_Pulse();
    LCD_delay(50000);
    
    
    //      initialization for matrix
    //taktirovanie
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

void LCD_WriteSTR(uint8_t *str) {
    
    uint8_t a;
    
    while (1) {
        a = *str;
        
        if(a) {
            LCD_Write_data (a);
        }
        else break;      
        str++;    
    }
    
        
}

void LCD_SetStartPos(uint8_t x, uint8_t y) {
    
    x = x - 1;
    x = x & 0x0F;
    x = 0x80 + x;
    
    if(y) {
        x = x | 0x40; 
    }
    
    LCD_WriteCMD(x);
}

void SysTick_Handler() {
    
    
    flag = 1;
    
//    k = 10000;
//    LED_8_ON();   
//    //LED_7_ON();
//    while(k--) {}
//    LED_7_OFF();
//    LED_8_OFF();
    
      
      
     // LCD_Write_data ('k');
}

//void GetTranslation (uint16_t x) {
//    
//   
//    uint8_t i = 0;
//    
//    while(x) {
//        m[i] = x % 10;
//        x = x/10;
//        i++;        
//    }    
//}

void Display (uint8_t koord_x, uint8_t koord_y, uint16_t numb){    
    
//        GetTranslation(numb);
    
    uint8_t i = 0;
    uint8_t m[5];
    
    while(numb) {
        m[i] = numb % 10;
        numb = numb/10;
        i++;        
    }   
         
        LCD_SetStartPos (koord_x,koord_y);
    
    
        LCD_Write_data ((0x30+m[3]));
        LCD_Write_data ((0x30+m[2]));
        LCD_Write_data ((0x30+m[1]));
        LCD_Write_data ((0x30+m[0]));
    
    
}

void GetADC () {
    if((ADC1 -> ISR) & ADC_ISR_EOC ) {
			
			DATA = ADC1 -> DR;
		}
    
}

int Degree2 (int deg) {
	
		int Degree2_out = 1;
		while (deg) {
			Degree2_out *= 2;
			deg--;
		}
		return Degree2_out;
}

int ColumnLevel (int Matr) { //input from 1 to 8
	
	int ColumnLevel_out = 0;
	
	while (Matr) {
		
		ColumnLevel_out = ColumnLevel_out + Degree2(Matr+7);
		Matr--;		
	}
	
	return ColumnLevel_out;
	
}

void Burn_Column_Level (int column, int level) {
	
		uint16_t Burn_slovo = Degree2(column-1) + ColumnLevel (level);
		
	
	  GPIOA -> BSRR = GPIO_BSRR_BR_8;
    
    SPI2->DR = Burn_slovo;
    while( (SPI2->SR & SPI_SR_BSY) == SPI_SR_BSY) {        
    }
    
    GPIOA -> BSRR = GPIO_BSRR_BS_8;	
}


int main (void) {
    
    int count = 0;
    uint16_t slovo;
    int p = 0;
	  int r = 0;
    static uint16_t mytime = 0;    
    uint8_t h;

		int mas[8];
	
	for (p=0; p<8; p++) {
		mas[p] = 0;
	
	}
	
    LCD_Config();
    
    SysTick_Config(SystemCoreClock); //interruption
    
    
    
    LCD_WriteCMD(0x80);
    LCD_delay(10000);
    //LCD_Write_data ('A');
    
    /*LCD_WriteSTR("STR1AbCD");
    
    LCD_WriteCMD(0xC0);
    LCD_WriteSTR("STR2AbCD");
    */
    
//    GetTranslation(3256);
//    
//    LCD_SetStartPos (1,0);    
//    LCD_Write_data ((0x30+m[3]));
//    
//    LCD_SetStartPos (2,0);    
//    LCD_Write_data ((0x30+m[2]));
//    
//    LCD_SetStartPos (3,0);    
//    LCD_Write_data ((0x30+m[1]));
//    
//    LCD_SetStartPos (4,0);    
//    LCD_Write_data ((0x30+m[0]));
//    
//    LCD_SetStartPos (1,1);
    
    
		
//		GPIOC->MODER &= ~ (GPIO_MODER_MODER6 | GPIO_MODER_MODER8 | GPIO_MODER_MODER7 | GPIO_MODER_MODER9);
//		GPIOC->MODER |= GPIO_MODER_MODER6_0 | GPIO_MODER_MODER8_0| GPIO_MODER_MODER7_0| GPIO_MODER_MODER9_0 ; //01
//		
//		GPIOC->PUPDR &= ~ (GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR7| GPIO_PUPDR_PUPDR9);


//		GPIOA -> PUPDR &= ~ GPIO_PUPDR_PUPDR8;
			//GPIOA -> PUPDR |= GPIO_PUPDR_PUPDR8_0; //PUPDR!!!
		
////    GPIOA -> BSRR = GPIO_BSRR_BS_8;
////		
////    for(p = 0; p < 16; p++) {       
////        GPIOB -> BSRR = GPIO_BSRR_BR_15;
////        
////        GPIOB -> BSRR = GPIO_BSRR_BS_13;
////        LCD_delay(10);
////        GPIOB -> BSRR = GPIO_BSRR_BR_13;
////        
////    }
////    GPIOA -> BSRR = GPIO_BSRR_BR_8;
    
    
    GetADC();
    Display(1, 1, DATA);
    
    mytime = 0;
    flag = 1;   
		
		//slovo = 0x0100;
		//slovo = slovo + 1+2+4+8+16+32+64+128;
		//slovo = slovo + Degree2(0) + Degree2(1) + Degree2(2);
		
		 //slovo = 0x0001;
		 //slovo = slovo + Degree2(8);
		 //slovo = slovo + ColumnLevel (8);
		
		
		
//    GPIOA -> BSRR = GPIO_BSRR_BR_8;
//    
//    SPI2->DR = slovo;
//    while( (SPI2->SR & SPI_SR_BSY) == SPI_SR_BSY) {        
//    }
//    
//    GPIOA -> BSRR = GPIO_BSRR_BS_8;
			
			//Burn_Column_Level (5);
			
    
    
    
    while(1) {
        
        if(flag == 1) {
            
						Display(1, 0, mytime);				           
            mytime++;
            flag = 0;
					
					
					for(r=0; r<7; r++) {						
						mas[7-r] = mas [6-r];					
					}
					
					if (								  (tmpDATA<=512 )	)	{mas[0] = 1; }	else {}
					if (	(	 512<tmpDATA)&(tmpDATA<=1024)	)	{mas[0] = 2; }	else {}
					if (	(	1024<tmpDATA)&(tmpDATA<=1536)	)	{mas[0] = 3; }	else {}
					if (	( 1536<tmpDATA)&(tmpDATA<=2048)	)	{mas[0] = 4; }	else {}
					if (	( 2048<tmpDATA)&(tmpDATA<=2560)	)	{mas[0] = 5; }	else {}
					if (	( 2560<tmpDATA)&(tmpDATA<=3072)	)	{mas[0] = 6; }	else {}
					if (	( 3072<tmpDATA)&(tmpDATA<=3584)	)	{mas[0] = 7; }	else {}
					if (	( 3584<tmpDATA)&(tmpDATA<=4095)	)	{mas[0] = 8; }	else {}
						
						
								
					
        }
				
				for(p=0; p<8; p++) {
							
							Burn_Column_Level (p+1,mas[p]);						
						}
	
        
       
        GetADC();
        if(DATA!=tmpDATA) {
            
            GetADC();
            Display(1, 1, DATA);            
            tmpDATA = DATA;            
        }
				
//				if (								 (tmpDATA<=512 )	)	{Burn_Column_Level (1,1);}	else {}
//				if (	(	 512<tmpDATA)&(tmpDATA<=1024)	)	{Burn_Column_Level (1,2);}	else {}
//				if (	(	1024<tmpDATA)&(tmpDATA<=1536)	)	{Burn_Column_Level (1,3);}	else {}
//				if (	( 1536<tmpDATA)&(tmpDATA<=2048)	)	{Burn_Column_Level (1,4);}	else {}
//				if (	( 2048<tmpDATA)&(tmpDATA<=2560)	)	{Burn_Column_Level (1,5);}	else {}
//				if (	( 2560<tmpDATA)&(tmpDATA<=3072)	)	{Burn_Column_Level (1,6);}	else {}
//				if (	( 3072<tmpDATA)&(tmpDATA<=3584)	)	{Burn_Column_Level (1,7);}	else {}
//				if (	( 3584<tmpDATA)&(tmpDATA<=4095)	)	{Burn_Column_Level (1,8);}	else {}
					
				if (								 (tmpDATA<=512 )	)	{mas[0] = 1; }	else {}
				if (	(	 512<tmpDATA)&(tmpDATA<=1024)	)	{mas[0] = 2; }	else {}
				if (	(	1024<tmpDATA)&(tmpDATA<=1536)	)	{mas[0] = 3; }	else {}
				if (	( 1536<tmpDATA)&(tmpDATA<=2048)	)	{mas[0] = 4; }	else {}
				if (	( 2048<tmpDATA)&(tmpDATA<=2560)	)	{mas[0] = 5; }	else {}
				if (	( 2560<tmpDATA)&(tmpDATA<=3072)	)	{mas[0] = 6; }	else {}
				if (	( 3072<tmpDATA)&(tmpDATA<=3584)	)	{mas[0] = 7; }	else {}
				if (	( 3584<tmpDATA)&(tmpDATA<=4095)	)	{mas[0] = 8; }	else {}
				
		
    };

        
 //matrix
    
    //LE 1
    
    
    
    
    
    
}




























/*int main (void) {
	
	Port_A_ON();
	Port_C_ON();
	
	//taktirovanie
	RCC -> APB2ENR |= RCC_APB2ENR_ADCEN;
	
	//calibration
	ADC1 -> CR = ADC_CR_ADCAL;
	while (ADC1 -> CR == ADC_CR_ADCAL) {
	}
	
	//turn on
	ADC1 -> CR = ADC_CR_ADEN;
	
	//ready?
	while (!(ADC1 -> ISR & ADC_ISR_ADRDY)){
	}
	
	//
	ADC1 -> CFGR1 |= ADC_CFGR1_CONT;
		
	//
	ADC1 -> CHSELR = ADC_CHSELR_CHSEL1;
	
	//
	ADC1 -> CR |= ADC_CR_ADSTART;
	
	
	
	//out pin
	GPIOC->MODER &= ~ (GPIO_MODER_MODER6 | GPIO_MODER_MODER8 | GPIO_MODER_MODER7 | GPIO_MODER_MODER9| GPIO_MODER_MODER12);
	GPIOC->MODER |= GPIO_MODER_MODER6_0 | GPIO_MODER_MODER8_0| GPIO_MODER_MODER7_0| GPIO_MODER_MODER9_0| GPIO_MODER_MODER12_0  ; //01
    
    GPIOA->MODER &= ~ GPIO_MODER_MODER15;
    GPIOA->MODER |= GPIO_MODER_MODER15_0;
		
	//o type
	GPIOC->OTYPER &= ~ (GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_7 | GPIO_OTYPER_OT_9);	
	
	//speed
	GPIOC->OSPEEDR &= ~ (GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR7 | GPIO_OSPEEDER_OSPEEDR9);		
	
	//pull up pull down
	GPIOC->PUPDR &= ~   (GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR7| GPIO_PUPDR_PUPDR9);
    GPIOA->PUPDR &= ~   (GPIO_PUPDR_PUPDR4 | GPIO_PUPDR_PUPDR5);
    GPIOA->PUPDR |=     (GPIO_PUPDR_PUPDR4_1 | GPIO_PUPDR_PUPDR5_1);
	
	
	while(1) {	
		
		if((ADC1 -> ISR) & ADC_ISR_EOC ) {
			
			DATA = ADC1 -> DR;
		}
		
		if((100<DATA)&&(DATA<1365)) {
			LED_6_ON();		
		}
		
		if((1365<DATA)&&(DATA<2730)) {
			LED_6_ON();
			LED_7_ON();			
		}
		
		if(2730<=DATA) {
			LED_6_ON();
			LED_7_ON();
			LED_8_ON();			
		}
		
		LED_6_OFF();
		LED_7_OFF(); 
		LED_8_OFF();	
	}

}

*/

/*
#include "stm32f0xx.h" //xx for family

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

#define PA15_ON()						(GPIOA -> BSRR = GPIO_BSRR_BS_15)
#define PA15_OFF()						(GPIOA -> BSRR = GPIO_BSRR_BR_15)
#define PC12_ON()						(GPIOC -> BSRR = GPIO_BSRR_BS_12)
#define PC12_OFF()						(GPIOC -> BSRR = GPIO_BSRR_BR_12)
#define PA45_in()                       (GPIOA -> IDR & (GPIO_IDR_4 |GPIO_IDR_5 ))

#define Port_A_ON()						(RCC -> AHBENR |= RCC_AHBENR_GPIOAEN)
#define Port_C_ON()						(RCC -> AHBENR |= RCC_AHBENR_GPIOCEN)

uint32_t DATA = 0;



int main (void) {
	
	Port_A_ON();
	Port_C_ON();
	
	//taktirovanie
	RCC -> APB2ENR |= RCC_APB2ENR_ADCEN;
	
	//calibration
	ADC1 -> CR = ADC_CR_ADCAL;
	while (ADC1 -> CR == ADC_CR_ADCAL) {
	}
	
	//turn on
	ADC1 -> CR = ADC_CR_ADEN;
	
	//ready?
	while (!(ADC1 -> ISR & ADC_ISR_ADRDY)){
	}
	
	//
	ADC1 -> CFGR1 |= ADC_CFGR1_CONT;
		
	//
	ADC1 -> CHSELR = ADC_CHSELR_CHSEL1;
	
	//
	ADC1 -> CR |= ADC_CR_ADSTART;
	
	
	
	//out pin
	GPIOC->MODER &= ~ (GPIO_MODER_MODER6 | GPIO_MODER_MODER8 | GPIO_MODER_MODER7 | GPIO_MODER_MODER9| GPIO_MODER_MODER12);
	GPIOC->MODER |= GPIO_MODER_MODER6_0 | GPIO_MODER_MODER8_0| GPIO_MODER_MODER7_0| GPIO_MODER_MODER9_0| GPIO_MODER_MODER12_0  ; //01
    
    GPIOA->MODER &= ~ GPIO_MODER_MODER15;
    GPIOA->MODER |= GPIO_MODER_MODER15_0;
		
	//o type
	GPIOC->OTYPER &= ~ (GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_7 | GPIO_OTYPER_OT_9);	
	
	//speed
	GPIOC->OSPEEDR &= ~ (GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR7 | GPIO_OSPEEDER_OSPEEDR9);		
	
	//pull up pull down
	GPIOC->PUPDR &= ~   (GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR7| GPIO_PUPDR_PUPDR9);
    GPIOA->PUPDR &= ~   (GPIO_PUPDR_PUPDR4 | GPIO_PUPDR_PUPDR5);
    GPIOA->PUPDR |=     (GPIO_PUPDR_PUPDR4_1 | GPIO_PUPDR_PUPDR5_1);
	
	
	while(1) {	
		
		if((ADC1 -> ISR) & ADC_ISR_EOC ) {
			
			DATA = ADC1 -> DR;
		}
		
		if((100<DATA)&&(DATA<1365)) {
			LED_6_ON();		
		}
		
		if((1365<DATA)&&(DATA<2730)) {
			LED_6_ON();
			LED_7_ON();			
		}
		
		if(2730<=DATA) {
			LED_6_ON();
			LED_7_ON();
			LED_8_ON();			
		}
		
		LED_6_OFF();
		LED_7_OFF(); 
		LED_8_OFF();	
	}

}
*/













































/*
uint32_t KEY_REG_TMP = 0;
uint32_t KEY_REG;
int counter = 0;
int N = 10000L;
int i;
void SysTick_Handler() {
	
    PA15_ON();
    //for(i=0;i<10;i++);    
    KEY_REG_TMP = PA45_in();
    KEY_REG = KEY_REG_TMP >> 4;
    PA15_OFF();
    
    
    PC12_ON();
    //for(i=0;i<10;i++);
    KEY_REG_TMP = PA45_in();
    KEY_REG = (KEY_REG | (KEY_REG_TMP)) >> 4;
    PC12_OFF();


}

int main (void) {
	
	Port_A_ON();	
	Port_C_ON();
	
	//out pin
	GPIOC->MODER &= ~ (GPIO_MODER_MODER6 | GPIO_MODER_MODER8 | GPIO_MODER_MODER7 | GPIO_MODER_MODER9| GPIO_MODER_MODER12);
	GPIOC->MODER |= GPIO_MODER_MODER6_0 | GPIO_MODER_MODER8_0| GPIO_MODER_MODER7_0| GPIO_MODER_MODER9_0| GPIO_MODER_MODER12_0  ; //01
    
    GPIOA->MODER &= ~ GPIO_MODER_MODER15;
    GPIOA->MODER |= GPIO_MODER_MODER15_0;
		
	//o type
	GPIOC->OTYPER &= ~ (GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_7 | GPIO_OTYPER_OT_9);	
	
	//speed
	GPIOC->OSPEEDR &= ~ (GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR7 | GPIO_OSPEEDER_OSPEEDR9);		
	
	//pull up pull down
	GPIOC->PUPDR &= ~   (GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR7| GPIO_PUPDR_PUPDR9);
    GPIOA->PUPDR &= ~   (GPIO_PUPDR_PUPDR4 | GPIO_PUPDR_PUPDR5);
    GPIOA->PUPDR |=     (GPIO_PUPDR_PUPDR4_1 | GPIO_PUPDR_PUPDR5_1);
	
	//in pin
	GPIOA->MODER &= ~ GPIO_MODER_MODER0;
        
    GPIOA->MODER &= ~ GPIO_MODER_MODER4;
    GPIOA->MODER &= ~ GPIO_MODER_MODER5;

    SysTick_Config(SystemCoreClock/1000); //interruption
     
    while(1) {
        
        if((KEY_REG & 0x1) == 0x1) {
            LED_6_ON();
        }
        
        if((KEY_REG & 0x2) == 0x2) {
            LED_7_ON();
        }
        
        if((KEY_REG & 0x4) == 0x4) {
            LED_8_ON();
        }
        
        if((KEY_REG & 0x8) == 0x8) {
            LED_9_ON();
        }
        
        
        for(i=0;i<N;i++);
        LED_6_OFF();
        LED_7_OFF();
        LED_8_OFF();
        LED_9_OFF();
		
		}
		

}
*/
