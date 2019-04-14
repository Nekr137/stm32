#include <stm32f0xx.h>

void StudyNVIC(void);
void delay (int a);


//void EXTI0_1_IRQHandler(void)
//{
//    if( (EXTI->IMR & EXTI_IMR_MR0) && (EXTI->PR & EXTI_PR_PR0))
//        {
//					 EXTI0Flag = 1;
//					 delay(50000);
//					 while(GPIOA->IDR & GPIO_IDR_0){}
//					 EXTI->PR |= EXTI_PR_PR0 ;
//        }
//}

//void EXTI0_1_IRQHandler(void)
//{
//	if((GPIOA->IDR & GPIO_IDR_0))
//	{
//		 EXTI0Flag = 1;
//		 delay(50000);
//		 while(GPIOA->IDR & GPIO_IDR_0){}
//		 EXTI->PR |= EXTI_PR_PR0 ;
//	}
//}


