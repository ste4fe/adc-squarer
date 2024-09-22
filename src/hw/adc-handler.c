#include "adc-handler.h"


HW_ADC_LLHander *HW_ADC_CurrentHandler = NULL;


void ADC1_2_IRQHandler(void) {
    ADC_ClearIntFlag(ADC1, ADC_FLAG_EOC);

    if (HW_ADC_CurrentHandler != NULL) {
        HW_ADC_CurrentHandler->conversionValue = ADC_ReadConversionValue(ADC1);
    }
}