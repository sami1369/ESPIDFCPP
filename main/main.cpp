#include <iostream>

#include "cppgpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


extern "C" void app_main(void) {
  CPPGPIO led1{GPIO_NUM_2};

  while (true) {
    // Turn the LED on
    led1.setPin();
    vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay for 1 second

    // Turn the LED off
    led1.clearPin();
    vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay for 1 second
  }
}