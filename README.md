# ESPIDFCPP - C++ Development with ESP-IDF

Welcome to ESPIDFCPP! This repository is designed as a learning project to familiarize with C++ programming in the ESP-IDF framework and contains a library implementation for the One-Wire protocol.

## Overview

This project aims to explore the use of C++ in the ESP-IDF environment, combining modern C++ programming techniques with the rich capabilities of ESP-IDF. The repository includes a library to interact with One-Wire devices, such as temperature sensors, on ESP32 hardware.

## Features

Implementation of One-Wire protocol for use with ESP32 GPIO pins.

Written entirely in C++, leveraging ESP-IDF for hardware interaction.

Clean and modular design, making it easy to extend and integrate into other ESP-IDF projects.





## Example usage:
```CPP
#include "OneWire.h"
extern "C" void app_main() {
    OneWire oneWire(GPIO_NUM_4);  // Use GPIO 4 for One-Wire communication
    oneWire.init();

    float temperature = oneWire.readTemperature();
    printf("Temperature: %.2f\n", temperature);
}
```


