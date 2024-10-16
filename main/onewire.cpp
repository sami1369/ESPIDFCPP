#include "onewire.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_timer.h"

OneWire::OneWire(CPPGPIO gpio) : mGPIO(gpio){}

bool OneWire::oneWireReset() {
  // Step 1: Pull the line low for 480 µs to reset devices
  mGPIO.clearPin();
  ets_delay_us(480);

  // Release the line and wait for the presence pulse
  mGPIO.setPin();
  ets_delay_us(70); 

  // Read the line to check for the presence pulse
  bool presence = mGPIO.read() == 0;
  ets_delay_us(410); // Wait for the remainder of the 480 µs

  return presence; // Return true if the device responded with a presence pulse
}

void OneWire::writeByte(uint8_t data) {
  for (int i = 0; i < 8; i++) {
    bool bit = (data >> i) & 0x01; // Extract the LSB
    if (bit) {
      // Write '1': Pull low for 1-15 µs, then release
      mGPIO.clearPin();
      ets_delay_us(1);
      mGPIO.setPin();
      ets_delay_us(60);
    } else {
      // Write '0': Pull low for 60 µs
      mGPIO.clearPin();
      ets_delay_us(60);
      mGPIO.setPin();
      ets_delay_us(1);
    }
  }
}

bool OneWire::readBit() {
  // Step 1: Pull the line low for 1-2 µs to initiate read
  mGPIO.clearPin();
  ets_delay_us(1);

  // Step 2: Release the line and wait for the slave to pull it low if it wants
  // to send a '0'
  mGPIO.setPin();
  ets_delay_us(
      14); // Wait for the slave response (usually between 15 and 60 µs)

  // Step 3: Read the line to get the bit value
  bool bit = mGPIO.read();

  // Step 4: Wait to finish the read slot timing
  ets_delay_us(45); // Complete the 60 µs read slot

  return bit; // Return the bit value (1 or 0)
}

uint8_t OneWire::readByte() {
  uint8_t data = 0;
  for (int i = 0; i < 8; i++) {
    if (readBit()) {
      data |= (1 << i); // Set the i-th bit to 1 if the read bit is 1
    }
  }
  return data;
}