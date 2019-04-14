
#include <stm32f0xx.h>

void DACconfig(void);
void DACwrite(uint16_t value);
uint16_t* My64sin(uint16_t * my_sin);

void DACsin(float points,uint32_t time_in_ms, uint16_t lines);
void MySIN(float x);			//draw sin using math.h




