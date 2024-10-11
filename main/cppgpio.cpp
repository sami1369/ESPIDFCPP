#include "cppgpio.h"

CPPGPIO::CPPGPIO(gpio_num_t pin) : mPin(pin) {

  // Configure GPIO
  gpio_config_t io_conf = {};
  io_conf.pin_bit_mask = (1ULL << mPin);
  io_conf.mode = GPIO_MODE_OUTPUT;
  io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
  io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  io_conf.intr_type = GPIO_INTR_DISABLE;
  gpio_config(&io_conf);
}

esp_err_t CPPGPIO::setPin() { return gpio_set_level(mPin, 1); }

esp_err_t CPPGPIO::clearPin() { return gpio_set_level(mPin, 0); }