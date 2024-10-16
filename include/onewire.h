#pragma once
#include "cppgpio.h"

class OneWire {
public:
            OneWire(CPPGPIO gpio);  //constructor
bool        oneWireReset();
void        writeByte(uint8_t data);
bool        readBit();
uint8_t     readByte();

private:
CPPGPIO     mGPIO;
};


