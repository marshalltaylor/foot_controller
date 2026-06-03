#include "segments.h"
#include <Arduino.h>
#include <Wire.h>

void
Segments::init(uint8_t i2c_addr)
{
    Serial.print("init 7seg at: ");
    Serial.println(i2c_addr);
    s7sAddress = i2c_addr;
    // Clear the display, and then turn on all segments and decimals
    clearDisplayI2C();     // Clears display, resets cursor
    setBrightnessI2C(255); // High brightness
}

// This custom function works somewhat like a serial.print.
//  You can send it an array of chars (string) and it'll print
//  the first 4 characters in the array.
void
Segments::s7sSendStringI2C(String toSend)
{
    Wire.beginTransmission(s7sAddress);
    for (int i = 0; i < 4; i++)
    {
        Wire.write(toSend[i]);
    }
    Wire.endTransmission();
}

// Send the clear display command (0x76)
//  This will clear the display and reset the cursor
void
Segments::clearDisplayI2C()
{
    Wire.beginTransmission(s7sAddress);
    Wire.write(0x76); // Clear display command
    Wire.endTransmission();
}

// Set the displays brightness. Should receive byte with the value
//  to set the brightness to
//  dimmest------------->brightest
//     0--------127--------255
void
Segments::setBrightnessI2C(byte value)
{
    Wire.beginTransmission(s7sAddress);
    Wire.write(0x7A);  // Set brightness command byte
    Wire.write(value); // brightness data byte
    Wire.endTransmission();
}

// Turn on any, none, or all of the decimals.
//  The six lowest bits in the decimals parameter sets a decimal
//  (or colon, or apostrophe) on or off. A 1 indicates on, 0 off.
//  [MSB] (X)(X)(Apos)(Colon)(Digit 4)(Digit 3)(Digit2)(Digit1)
void
Segments::setDecimalsI2C(byte decimals)
{
    Wire.beginTransmission(s7sAddress);
    Wire.write(0x77);
    Wire.write(decimals);
    Wire.endTransmission();
}

void
Segments::write_u8(uint8_t data)
{
    char tempString[10]; // Will be used with sprintf to create strings
    sprintf(tempString, "%4d", data);
    // This will output the tempString to the S7S
    s7sSendStringI2C(tempString);
}
