#include "apm32f10x_gpio.h"

namespace HW::GPIO {
class Pin {
   public:
    Pin(GPIO_T *port, GPIO_PIN_T pin);

    void config(GPIO_SPEED_T speed, GPIO_MODE_T mode);

   private:
    GPIO_T *_port;
    GPIO_PIN_T _pin;
};
}  // namespace HW::GPIO
