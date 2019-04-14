
#include "MyNVIC.h"


void StudyNVIC(void)
{
    SYSCFG->EXTICR[1] &= (0x000F);					// 1. to chose PA0

    EXTI->RTSR = EXTI_RTSR_TR0; 				    // 2. Set interrupt trigger to rising edge

    EXTI->IMR = EXTI_IMR_MR0; 							// 3. unmask EXTI0 line
    //NVIC_SetPriority(EXTI0_1_IRQn, 1);	 	// 4. Set Priority to 1
    NVIC_EnableIRQ(EXTI0_1_IRQn);  					// 5. Enable EXTI0_1 interrupt in NVIC (do 4 first)
}

void delay (int a)
{
    volatile int i,j;

    for (i=0 ; i < a ; i++)
    {
        j++;
    }

    return;
}