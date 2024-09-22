#pragma once

#include <stdint.h>

#include "apm32f10x_adc.h"

#include "adc-handler.h"

namespace HW {
class ADCSensor {
   public:
    void init(ADC_CHANNEL_T channel);
    uint16_t getResult();

   private:
    HW_ADC_LLHander LLHandlerInstance = {.conversionValue = 0};
};

extern ADCSensor ADCSensorInstance;
}  // namespace HW
