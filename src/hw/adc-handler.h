#pragma once

#include "apm32f10x_adc.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Low level adc handler.
 * 
 * @details Поздно вспомнил что у моего мк есть только один ацп.
 * 
 */
typedef struct HW_ADC_LLHander {
    // ADC_T *adc;
    uint16_t conversionValue;
} HW_ADC_LLHander;

extern HW_ADC_LLHander *HW_ADC_CurrentHandler;

#ifdef __cplusplus 
}
#endif