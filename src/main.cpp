#include <string.h>

#include "hw/adc.h"
#include "hw/pins.h"
#include "hw/sysclk.h"
#include "hw/usart.h"

#include "prototreads/pt.h"
#include "util.h"

float SquareResult = 0;
uint16_t ADCSensorMillivolts = 0;

PT_THREAD(thread_ADCChecker(struct pt *pt)) {
    PT_BEGIN(pt);

    while (true) {
        ADCSensorMillivolts = (HW::ADCSensorInstance.getResult() * 3300) / 4095;

        PT_YIELD(pt);
    }

    PT_END(pt);
}

PT_THREAD(thread_SQRTCalculator(struct pt *pt)) {
    PT_BEGIN(pt);

    static uint32_t timestamp = 0;

    while (true) {
        PT_WAIT_UNTIL(
            pt, (HW::SysCLK::getTime() - timestamp < 1000 /*Каждую секунду*/)
        );

        timestamp = HW::SysCLK::getTime();

        SquareResult = util::calculateSqare((float)ADCSensorMillivolts);

        PT_YIELD(pt);
    }

    PT_END(pt);
}

PT_THREAD(thread_USARTHandler(struct pt *pt)) {
    PT_BEGIN(pt);

    static HW::USARTConsole *console = &HW::USARTConsoleInstance;
    static char bffer[HW::USARTbufferSize] = {0};

    while (true) {
        PT_WAIT_UNTIL(pt, console->getLine(bffer));

        PT_WAIT_WHILE(pt, console->busy());

        if (strcmp(bffer, "callback") == 0) {
            // Device callback
            console->loadAnswer("accept\n");
        } else if (strcmp(bffer, "getVoltage") == 0) {
            // Get adc voltage
            memset(bffer, 0, 10);
            
            util::halfWord2AsciiString(ADCSensorMillivolts, bffer);
            strcat(bffer, "\n");

            console->loadAnswer(bffer);

        } else if (strcmp(bffer, "getSquare") == 0) {
            // Get square of voltage.
            
            memset(bffer, 0, sizeof(bffer));

            util::float2Ascii(SquareResult, bffer);
            strcat(bffer, "\n");

            console->loadAnswer(bffer);
        }

        memset(bffer, 0, sizeof(bffer));
    }

    PT_END(pt);
}

int main() {
    struct pt threadDispatcher;
    uint32_t timestamp = 0;
    HW::ADCSensor &adc = HW::ADCSensorInstance;

    HW::SysCLK::init();
    HW::Pins::USARTPinTX.config(GPIO_SPEED_50MHz, GPIO_MODE_AF_PP);
    HW::Pins::USARTPinRX.config(GPIO_SPEED_50MHz, GPIO_MODE_IN_FLOATING);
    HW::Pins::ADCPin.config(GPIO_SPEED_50MHz, GPIO_MODE_ANALOG);

    HW::USARTConsoleInstance.init(HW::defaultUSART, HW::defaultRxDMA, HW::defaultTxDMA);
    adc.init(ADC_CHANNEL_0);

    PT_INIT(&threadDispatcher);

    while (true) {
        thread_ADCChecker(&threadDispatcher);
        thread_SQRTCalculator(&threadDispatcher);
        thread_USARTHandler(&threadDispatcher);
    }

    return 0;
}
