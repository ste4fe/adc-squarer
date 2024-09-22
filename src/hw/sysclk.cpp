#include "sysclk.h"

#include <apm32f10x_rcm.h>
#include "systick.h"

void HW::SysCLK::init() {
    SystemInit();
    SystemCoreClockUpdate();
    SysTick_Config((SystemCoreClock / 1000) - 1);
    NVIC_EnableIRQ(SysTick_IRQn);

    RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_DMA1 | RCM_AHB_PERIPH_FPU);
    RCM_EnableAPB2PeriphClock(
        RCM_APB2_PERIPH_USART1 | RCM_APB2_PERIPH_ADC1 | RCM_APB2_PERIPH_GPIOA
    );
}

uint32_t HW::SysCLK::getTime() {
    return HW_Systick_Counter;
}

void HW::SysCLK::hardDelay(uint32_t ms) {
    uint32_t start = getTime();

    while (getTime() - start < ms) {}
}
