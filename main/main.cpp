#include <iostream>

#include "cppgpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern "C" void app_main(void) {
  // Step 1: Initialize GPIO for One-Wire Protocol
  CPPGPIO oneWire{GPIO_NUM_15, GPIO_MODE_INPUT_OUTPUT_OD, GPIO_PULLUP_ENABLE};

  while (true) {
    // Step 2: Send Reset Pulse and Check for Presence
    // Pull the line low for 480 µs to reset devices
    oneWire.clearPin();
    ets_delay_us(480);

    // Release the line and wait for the presence pulse
    oneWire.setPin();
    ets_delay_us(70); // Wait for at least 60 µs for presence pulse

    // Read the line to check for the presence pulse
    bool presence = oneWire.read() == 0;
    ets_delay_us(410); // Wait for the remainder of the 480 µs

    if (presence) {
      printf("Device detected on one-wire bus.\n");
    } else {
      printf("No device found on one-wire bus.\n");
    }
    vTaskDelay(pdMS_TO_TICKS(1000));  // Delay for 1 second before retrying

  }
}