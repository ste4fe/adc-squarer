#include "systick.h"

uint32_t HW_Systick_Counter = 0;

void SysTick_Handler(void) {
    HW_Systick_Counter++;
}