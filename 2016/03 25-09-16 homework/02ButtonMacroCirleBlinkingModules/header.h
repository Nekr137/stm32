
#ifndef _header_H // lock the reinclusion of this module
#define _header_H

//******************************************************************************
// include section
//******************************************************************************
		#include "stm32f0xx.h"

//******************************************************************************
// section of constant defenition
//******************************************************************************

		//MY CONSTANTS
		#define MyModes 3											//LEDS Regimes

		//bool
		#define PRESSED 1 
		#define UNPRESSED 0 
		#define LIGHT 1
		#define NOTLIGHT 0

//******************************************************************************
// section of type defenition
//******************************************************************************
		//typedef struct
		//{
		// ...
		//} T_STRUCT;
		//typedef ...
//******************************************************************************
// section of global var defenition
//******************************************************************************
		//extern char GlobalVar1;
		//extern char GlobalVar2;
		//extern ...
//******************************************************************************
// section of global functions prototypes
//******************************************************************************
		//void global_func1 (void);
		//void global_func2 (void);
		//...
//******************************************************************************
// section of macros defenition
//******************************************************************************

		//MY BUTTON MACROS
		#define BUTTON_IN()			(GPIOA->IDR & GPIO_IDR_0)

		//MY LED macros
		#define SETLED6()				(GPIOC->BSRR = GPIO_BSRR_BS_6)
		#define SETLED9()				(GPIOC->BSRR = GPIO_BSRR_BS_9)
		#define SETLED7()				(GPIOC->BSRR = GPIO_BSRR_BS_7)
		#define SETLED8()				(GPIOC->BSRR = GPIO_BSRR_BS_8)
		#define RESETLED6()			(GPIOC->BSRR = GPIO_BSRR_BR_6)
		#define RESETLED9()			(GPIOC->BSRR = GPIO_BSRR_BR_9)
		#define RESETLED7()			(GPIOC->BSRR = GPIO_BSRR_BR_7)
		#define RESETLED8()			(GPIOC->BSRR = GPIO_BSRR_BR_8)
				
#endif // Closing #endif 

//******************************************************************************
// ENF OF FILE
//****************************************************************************** 
