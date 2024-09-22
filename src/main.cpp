#include "pt.h"

#include "hw/pins.h"
#include "hw/sysclk.h"
#include "hw/usart.h"

PT_THREAD(printHello(struct pt *pt)) {
    PT_BEGIN(pt);

    PT_END(pt);
}

int main() {
    HW::SysCLK::init();

    HW::Console.init();

    HW::Pins::USARTPins.config(GPIO_SPEED_50MHz, GPIO_MODE_AF_PP);

    while (true) {
        HW::Console.loadAnswer("pizda");
        HW::SysCLK::hardDelay(1000);
    }
    return 0;
}
