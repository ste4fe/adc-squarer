#include "pt.h"

#include "hw/pins.h"
#include "hw/sysclk.h"
#include "hw/usart.h"
#include "hw/adc.h"

PT_THREAD(printHello(struct pt *pt)) {
    PT_BEGIN(pt);

    PT_END(pt);
}

int main() {
    HW::ADCSensor &adc = HW::ADCSensorInstance;
    HW::SysCLK::init();
    HW::Pins::USARTPins.config(GPIO_SPEED_50MHz, GPIO_MODE_AF_PP);
    HW::Pins::ADCPin.config(GPIO_SPEED_50MHz, GPIO_MODE_ANALOG);

    HW::Console.init();
    adc.init(ADC_CHANNEL_0);

    while (true) {
        HW::Console.loadAnswer("pizda");
        HW::SysCLK::hardDelay(1000);
    }
    return 0;
}
