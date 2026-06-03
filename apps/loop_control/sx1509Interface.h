#ifndef SX1509_INTERFACE_H
#define SX1509_INTERFACE_H

#include "HardwareInterfaces.h"
#include <Arduino.h>
#include <PanelDataObjects.h>

// This is the most basic hardware type, the Arduino digital input
class sx1509DigitalIn : public GenericHardwareDescription
{
public:
    sx1509DigitalIn(int inputPin);
    void readHardware(void);

protected:
    int pin;
};

// Arduino digital out
class sx1509DigitalOut : public GenericHardwareDescription
{
public:
    sx1509DigitalOut(int inputPin);
    void writeHardware(void);

protected:
    int pin;
};

#endif