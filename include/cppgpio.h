#pragma once

#include "driver/gpio.h"

class CPPGPIO {
public:
CPPGPIO(gpio_num_t pin);  //constructor 
CPPGPIO(gpio_num_t pin, gpio_mode_t mode, gpio_pullup_t pullup); //constructor
esp_err_t setPin();
esp_err_t clearPin();
int read ();
private:
  gpio_num_t mPin;
};