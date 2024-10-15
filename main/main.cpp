#include <iostream>

#include "cppgpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

bool oneWireReset(CPPGPIO &gpio) {
  // Step 1: Pull the line low for 480 µs to reset devices
  // Pull the line low for 480 µs to reset devices
  gpio.clearPin();
  ets_delay_us(480);

  // Release the line and wait for the presence pulse
  gpio.setPin();
  ets_delay_us(70); // Wait for at least 60 µs for presence pulse

  // Read the line to check for the presence pulse
  bool presence = gpio.read() == 0;
  ets_delay_us(410); // Wait for the remainder of the 480 µs

  return presence; // Return true if the device responded with a presence pulse
}

void writeByte(CPPGPIO &gpio, uint8_t data) {
  for (int i = 0; i < 8; i++) {
    bool bit = (data >> i) & 0x01; // Extract the LSB
    if (bit) {
      // Write '1': Pull low for 1-15 µs, then release
      gpio.clearPin();
      ets_delay_us(1);
      gpio.setPin();
      ets_delay_us(60);
    } else {
      // Write '0': Pull low for 60 µs
      gpio.clearPin();
      ets_delay_us(60);
      gpio.setPin();
      ets_delay_us(1);
    }
  }
}

bool readBit(CPPGPIO &gpio) {
  // Step 1: Pull the line low for 1-2 µs to initiate read
  gpio.clearPin();
  ets_delay_us(1);

  // Step 2: Release the line and wait for the slave to pull it low if it wants
  // to send a '0'
  gpio.setPin();
  ets_delay_us(
      14); // Wait for the slave response (usually between 15 and 60 µs)

  // Step 3: Read the line to get the bit value
  bool bit = gpio.read();

  // Step 4: Wait to finish the read slot timing
  ets_delay_us(45); // Complete the 60 µs read slot

  return bit; // Return the bit value (1 or 0)
}

uint8_t readByte(CPPGPIO &gpio) {
  uint8_t data = 0;
  for (int i = 0; i < 8; i++) {
    if (readBit(gpio)) {
      data |= (1 << i); // Set the i-th bit to 1 if the read bit is 1
    }
  }
  return data;
}
extern "C" void app_main(void) {
  // Step 1: Initialize GPIO for One-Wire Protocol
  CPPGPIO oneWire{GPIO_NUM_15, GPIO_MODE_INPUT_OUTPUT_OD, GPIO_PULLUP_ENABLE};
  printf("GPIO initialized for One-Wire protocol.\n");
  while (true) {


    if (oneWireReset(oneWire)) {
      printf("Device detected on one-wire bus.\n");
      // Step 3: Write a Byte (Example: Sending a Reset Command)
      writeByte(oneWire, 0xCC); // Skip ROM command (0xCC)
      writeByte(oneWire, 0x44); // Convert Temperature command (0x44)

      // Delay for conversion time (750 ms typical for 12-bit resolution)
      oneWireReset(oneWire);
      // Step 6: Read Temperature Data (from scratchpad)
      writeByte(oneWire, 0xCC); // Skip ROM command
      writeByte(oneWire, 0xBE); // Read Scratchpad command

      // Now, read the temperature bytes (LSB and MSB)
      uint8_t tempLSB = readByte(oneWire);
      uint8_t tempMSB = readByte(oneWire);
      // Print the raw LSB and MSB values for debugging
     printf("Temp LSB: 0x%02X\n", tempLSB);
     printf("Temp MSB: 0x%02X\n", tempMSB);

    // Calculate temperature
    int16_t tempRaw = (tempMSB << 8) | tempLSB;
    float temperature = tempRaw / 16.0;
    printf("Temperature: %.2f°C\n", temperature);
    } 
    else {
      printf("No device found on one-wire bus.\n");
    }
    vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second before retrying

    // if (oneWire.read() == 0) {
    //   printf("Device detected on one-wire bus.\n");

    //   // Step 3: Write a Byte (Example: Sending a Reset Command)
    //   writeByte(oneWire, 0xCC); // Command to skip ROM

    // } else {
    //   printf("No device found on one-wire bus.\n");
    // }
    // vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second before retrying

  }
}