#include "gpio.h"

namespace HW::Pins {
GPIO::Pin USARTPins(GPIOA, (GPIO_PIN_T)(GPIO_PIN_9 | GPIO_PIN_10));
GPIO::Pin ADCPin(GPIOA, (GPIO_PIN_T)GPIO_PIN_0);
}  // namespace HW::Pins
