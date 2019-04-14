#include "stm32f0xx.h" 

uint16_t j;

#define blue_on() (GPIOC->BSRR = GPIO_BSRR_BS_7)
#define blue_off() (GPIOC->BSRR = GPIO_BSRR_BR_7)

#define red_on() (GPIOC->BSRR = GPIO_BSRR_BS_6)
#define red_off() (GPIOC->BSRR = GPIO_BSRR_BR_6)

#define green_on() (GPIOC->BSRR = GPIO_BSRR_BS_9)
#define green_off() (GPIOC->BSRR = GPIO_BSRR_BR_9)

#define yel_on() (GPIOC->BSRR = GPIO_BSRR_BS_8)
#define yel_off() (GPIOC->BSRR = GPIO_BSRR_BR_8)
/*
int RED_CNT=30000;
int RED_YEL_CNT=10000;
int YEL_CNT=10000;
int GREEN_CNT=30000;
int GREEN_BLK_CNT=30000;
int flag = 0;

uint16_t j;

int user_cnt = 30;

int flag1=0;

int TL_CNT=0;

int TICK_CNT;

int flag_sec;

int flag_msec;

void SysTick_Handler()
{
	
	j = GPIOA->IDR & 1;
	if (j == 1)
	{
		user_cnt++;
	}
	else 
	{
		user_cnt=0;
	}
	if (user_cnt == 30)
	{

		TL_CNT=1;
		flag=0;
		RED_CNT=30000;
		RED_YEL_CNT=10000;
		YEL_CNT=10000;
		GREEN_CNT=30000;
		GREEN_BLK_CNT=30000;
	}
	
	
	
	if(++TICK_CNT == 1000)
	{
		flag_msec = TICK_CNT;
		TICK_CNT=0;
		flag_sec =1;

	}
	
}
*/

int main(){
	
	// INC ACP
	RCC->APB2ENR |= RCC_APB2ENR_ADCEN;
	
	
	// INC CAL 
	ADC1->CR = ADC_CR_ADCAL;
		
	while (ADC1->CR & ADC_CR_ADCAL)
	{
	}
	
	// ADC EN
	ADC1->CR = ADC_CR_ADEN;
	
	while(!(ADC1->ISR & ADC_ISR_ADRDY)){}
	

	// INC CH SEL REG
	ADC1 ->CHSELR = ADC_CHSELR_CHSEL1;
	
		
	ADC1->CR |= ADC_CR_ADSTART;
	// INC CONT
	ADC1->CFGR1 = ADC_CFGR1_CONT;
	
	// ADC EN
	//ADC1->CR = ADC_CR_ADEN;
	
	
	//LEDS
	
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	
	
	GPIOC->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER8 | GPIO_MODER_MODER7 | GPIO_MODER_MODER9);
	GPIOC->MODER |= GPIO_MODER_MODER6_0 | GPIO_MODER_MODER8_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER9_0;
	
	GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7 | GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9);
	
	GPIOC->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEEDR6 | GPIO_OSPEEDR_OSPEEDR7 | GPIO_OSPEEDR_OSPEEDR8 | GPIO_OSPEEDR_OSPEEDR9);
	
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR7 | GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9);
	
	
	//POT
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	
	//GPIOA->MODER &= ~ GPIO_MODER_MODER1;
	GPIOA->MODER |= GPIO_MODER_MODER1;
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR1);
	
	
	while(1)
	{
		if((ADC1->ISR) & ADC_ISR_EOC)
		{
			j = ADC1->DR;
		}
		
		
		
		if(j <= 1024)
		{
			
			green_off();
			
			yel_off();
			red_off();
		}
		
		if((j>1024) && (j<=2048))
		{
			yel_off();
			
			red_off();
			green_on();
					
		}
		
		if((j>2048) && (j<=3072))
		{
			yel_off();
			
			red_on();
			green_on();
					
		}
		
		if(j>3072)
		{
			red_on();
			
			green_on();
			yel_on();
		}
		
	}
	
	
	
	
	
	/*
	//USER_BUTTON
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	
	
	GPIOA->MODER &= ~ GPIO_MODER_MODER0;
	
	GPIOA->OTYPER &= ~ GPIO_OTYPER_OT_0;
	
	GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEEDR0);
	
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR0);
	
	
	//LEDS
	
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	
	GPIOC->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER8 | GPIO_MODER_MODER7 | GPIO_MODER_MODER9);
	GPIOC->MODER |= GPIO_MODER_MODER6_0 | GPIO_MODER_MODER8_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER9_0;
	
	GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7 | GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9);
	
	GPIOC->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEEDR6 | GPIO_OSPEEDR_OSPEEDR7 | GPIO_OSPEEDR_OSPEEDR8 | GPIO_OSPEEDR_OSPEEDR9);
	
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR7 | GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9);
	*/
	/*
	SysTick_Config(SystemCoreClock/1000);	
	
	while(1){
		if (flag_sec == 1)
		{

			flag_sec =0;

		}
		if (flag1 == 1)
		{
			flag1 =0;
			RED_CNT=30000;
			RED_YEL_CNT=10000;
			YEL_CNT=10000;
			GREEN_CNT=30000;
			GREEN_BLK_CNT=30000;
		}
		
		if(TL_CNT == 0)
		{			
			//red
			if((RED_CNT != 0) && (flag==0)){red_on();RED_CNT--;}
			if((RED_CNT == 0) && (flag==0)){red_off();flag=1;}
			//red-yellow	
			if((RED_YEL_CNT != 0) && (flag==1)){red_on(); yel_on(); RED_YEL_CNT--;}
			if((RED_YEL_CNT == 0) && (flag==1)){red_off();yel_off();flag=2;}
			//green	
			if((GREEN_CNT != 0) && (flag==2)){green_on();GREEN_CNT--;}
			if((GREEN_CNT == 0) && (flag==2)){green_off();flag=3;}
			//blinking green	
			if((GREEN_BLK_CNT != 0) && (flag==3))
			{	
				if(GREEN_BLK_CNT <= 30000 && GREEN_BLK_CNT > 25000)
				{green_on(); GREEN_BLK_CNT--;}
				if(GREEN_BLK_CNT <= 25000 && GREEN_BLK_CNT > 20000)
				{green_off(); GREEN_BLK_CNT--;}
				if(GREEN_BLK_CNT <= 20000 && GREEN_BLK_CNT > 15000)
				{green_on(); GREEN_BLK_CNT--;}
				if(GREEN_BLK_CNT <= 15000 && GREEN_BLK_CNT > 10000)
				{green_off(); GREEN_BLK_CNT--;}
				if(GREEN_BLK_CNT <= 10000 && GREEN_BLK_CNT > 5000)
				{green_on(); GREEN_BLK_CNT--;}
				if(GREEN_BLK_CNT <= 5000 && GREEN_BLK_CNT > 0)
				{green_off(); GREEN_BLK_CNT--;}
			}	
			if((GREEN_BLK_CNT == 0) && (flag==3)){green_off(); flag=4;}
			//yellow
			if((YEL_CNT !=0) && (flag==4)){yel_on();YEL_CNT--;}
			if((YEL_CNT ==0) && (flag==4)){yel_off();flag=0;flag1=1;}
			

		}
		
		if(TL_CNT != 0)
		{
			//red-yellow	
			if((RED_YEL_CNT != 0) && (flag==0)){red_on(); yel_on(); RED_YEL_CNT--;}
			if((RED_YEL_CNT == 0) && (flag==0)){red_off();yel_off();flag=1;}
			//green	
			if((GREEN_CNT != 0) && (flag==1)){green_on();GREEN_CNT--;}
			if((GREEN_CNT == 0) && (flag==1)){green_off();flag=2;}
			//blinking green	
			if((GREEN_BLK_CNT != 0) && (flag==2))
			{	
				if(GREEN_BLK_CNT <= 30000 && GREEN_BLK_CNT > 25000)
				{green_on(); GREEN_BLK_CNT--;}
				if(GREEN_BLK_CNT <= 25000 && GREEN_BLK_CNT > 20000)
				{green_off(); GREEN_BLK_CNT--;}
				if(GREEN_BLK_CNT <= 20000 && GREEN_BLK_CNT > 15000)
				{green_on(); GREEN_BLK_CNT--;}
				if(GREEN_BLK_CNT <= 15000 && GREEN_BLK_CNT > 10000)
				{green_off(); GREEN_BLK_CNT--;}
				if(GREEN_BLK_CNT <= 10000 && GREEN_BLK_CNT > 5000)
				{green_on(); GREEN_BLK_CNT--;}
				if(GREEN_BLK_CNT <= 5000 && GREEN_BLK_CNT > 0)
				{green_off(); GREEN_BLK_CNT--;}
			}	
			if((GREEN_BLK_CNT == 0) && (flag==2)){green_off(); flag=3;}
			//yellow
			if((YEL_CNT !=0) && (flag==3)){yel_on();YEL_CNT--;}
			if((YEL_CNT ==0) && (flag==3)){yel_off();flag=0;flag1=1;TL_CNT=0;}
			
							
		}
		
	};
	
	*/
}



