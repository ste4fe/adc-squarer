/**
 * @file systick.c
 * @author your name (you@domain.com)
 * @brief Модуль-обработчик прерывания по Systick
 * @version 0.1
 * @date 2024-09-22
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "systick.h"

uint32_t HW_Systick_Counter = 0;

void SysTick_Handler(void) {
    HW_Systick_Counter++;
}