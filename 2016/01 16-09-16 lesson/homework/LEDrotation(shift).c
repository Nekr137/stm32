#include "stm32f0xx.h" 

void MyConfigurationRegisters()
{
    /// Configuration registers
    
        //GPIO port mode register
            GPIOC->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7 | GPIO_MODER_MODER8 | GPIO_MODER_MODER9);
            GPIOC->MODER |= (GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0);
        
        //GPIO port output type register (GPIOx_OTYPER)
            GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7 | GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9);
                
        //GPIO port output speed register (GPIOx_OSPEEDR) (x = A..F)
            GPIOC->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7 | GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9);
                
        //GPIO port pull-up/pull-down register (GPIOx_PUPDR) (x = A..F)
            GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR7 | GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9);
    ///
}

void MyRotation(int LightTime)
{
    //variables
      
    
    //work with configuration registers:
        MyConfigurationRegisters();
    
    //work with data registers: (LED: 6978 - clockwise )
        while(1)
        {
            GPIOC->ODR |= GPIO_ODR_6;
            GPIOC->ODR &= ~GPIO_ODR_6; 
            
            for(i=0;i<t*100000L;i++);
            
            GPIOC->ODR |= GPIO_ODR_9;
            GPIOC->ODR &= ~GPIO_ODR_9;
            
            for(i=0;i<t*100000L;i++);
            
            GPIOC->ODR |= GPIO_ODR_7;
            GPIOC->ODR &= GPIO_ODR_7;
                        
            for(i=0;i<t*100000L;i++);
            
                        
            GPIOC->ODR |= GPIO_ODR_8;
            GPIOC->ODR &= GPIO_ODR_8;
                        
            for(i=0;i<t*100000L;i++);
                         
        }   
}

int main(void)
{
    //variables
        static uint32_t LightTime;  
        
    //подаем такты на порт. Без этой команды ничего не работает.
        RCC->AHBENR |= RCC_AHBENR_GPIOCEN; //!! Нужно ли эту хрень вносить в мою функцию, или оставить в мэйне?     
    
    //LEDs rotation
        MyRotation(1);   
}

//то, что нужно попробовать:
//void Soft_Delay(volatile uint32_t number)
//{
//        while(number--);
//}
