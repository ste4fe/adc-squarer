#include "gpio.h"

HW::GPIO::Pin::Pin(GPIO_T *port, GPIO_PIN_T pin) {
    this->_pin = pin;
    this->_port = port;
}

void HW::GPIO::Pin::config(GPIO_SPEED_T speed, GPIO_MODE_T mode) {
    GPIO_Config_T GPIOConfig;

    GPIOConfig.mode = mode;
    GPIOConfig.speed = speed;
    GPIOConfig.pin = this->_pin;

    GPIO_Config(this->_port, &GPIOConfig);
}