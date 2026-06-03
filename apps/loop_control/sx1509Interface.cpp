#include "sx1509Interface.h"
#include "globals.h"
#include <Arduino.h>

// Note, the ought to be deconstructed if the hardware interfaces are used
// dynamically

// Arduino in
sx1509DigitalIn::sx1509DigitalIn(int inputPin)
{
    expander.pinMode(inputPin, INPUT_PULLUP);
    localData.size = 1;
    localData.data = new uint8_t[localData.size];
    pin            = inputPin;
}

void
sx1509DigitalIn::readHardware(void)
{
    *localData.data = expander.get_pin(pin);
};

// Arduino Digital out
sx1509DigitalOut::sx1509DigitalOut(int inputPin)
{
    expander.pinMode(inputPin, OUTPUT);
    localData.size = 1;
    localData.data = new uint8_t[localData.size];
    pin            = inputPin;
}

void
sx1509DigitalOut::writeHardware(void)
{
    expander.set_pin(pin, *localData.data);
};
