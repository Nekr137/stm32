


#include <stm32f0xx.h>

//for LCD
#define reset_rs()                      (GPIOB -> BSRR = GPIO_BSRR_BR_12)
#define set_rs()                        (GPIOB -> BSRR = GPIO_BSRR_BS_12)

#define reset_rw()                      (GPIOB -> BSRR = GPIO_BSRR_BR_3)
#define set_rw()                        (GPIOB -> BSRR = GPIO_BSRR_BS_3)

#define reset_en()                      (GPIOC -> BSRR = GPIO_BSRR_BR_4)
#define set_en()                        (GPIOC -> BSRR = GPIO_BSRR_BS_4)

void Half_Byte(uint8_t j);
void LCDinit(void);
void LCD_delay(uint16_t j);
void LCD_Pulse(void);
void LCD_Wait(void);
void LCD_WriteCMD(uint8_t j);
void LCD_Write_data(uint8_t j);
void LCD_Config(void);
void LCD_WriteSTR(uint8_t *str);
void LCD_SetStartPos(uint8_t x, uint8_t y);
void LCD_ShowNum2(uint16_t chislo,uint8_t x, uint8_t y);
void LCD_ShowNum16_MinusPlus(uint16_t chislo,uint8_t x, uint8_t y);
void LCD_Show_double_(double t,uint8_t x, uint8_t y);
void LCD_Show_double(double t,uint8_t x, uint8_t y);
void LCD_Rectangle(uint32_t x, uint32_t y);
void LCD_Battery(uint32_t level,uint32_t x, uint32_t y, uint8_t address);
