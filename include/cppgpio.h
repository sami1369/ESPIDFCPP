#pragma once

#include "driver/gpio.h"

class CPPGPIO {
public:
CPPGPIO(gpio_num_t pin);  //constructor 
esp_err_t setPin();
esp_err_t clearPin();
private:
  gpio_num_t mPin;
};