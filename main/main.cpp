#include <iostream>

#include "cppgpio.h"
#include "onewire.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


extern "C" void app_main(void) 
{
  // Step 1: Initialize GPIO for One-Wire Protocol
  CPPGPIO   gpio1{GPIO_NUM_15, GPIO_MODE_INPUT_OUTPUT_OD, GPIO_PULLUP_ENABLE};
  OneWire   oneWire(gpio1);
  while(true) 
  { 
    if(oneWire.oneWireReset())
    {
      printf("Device detected on one-wire bus.\n");

      oneWire.writeByte(0xCC); // Skip ROM command (0xCC)
      oneWire.writeByte(0x44); // Convert Temperature command (0x44)

      // Delay for conversion time (1000 ms typical for 12-bit resolution)
      vTaskDelay(pdMS_TO_TICKS(1000));

      oneWire.oneWireReset();

      oneWire.writeByte(0xCC); // Skip ROM command (0xCC)
      oneWire.writeByte(0xBE); // Convert Temperature command (0x44)

      // Now, read the temperature bytes (LSB and MSB)
      uint8_t tempLSB = oneWire.readByte();
      uint8_t tempMSB = oneWire.readByte();

      // Print the raw LSB and MSB values for debugging
      printf("Temp LSB: 0x%02X\n", tempLSB);
      printf("Temp MSB: 0x%02X\n", tempMSB);

      // Calculate temperature
      int16_t tempRaw = (tempMSB << 8) | tempLSB;
      float temperature = tempRaw / 16.0;
      printf("Temperature: %.2f°C\n", temperature);
    } 
    else 
    {
      printf("Device not detected on one-wire bus.\n");
    }
  }
}