#ifndef SEGMENTS_H
#define SEGMENTS_H

#include <Arduino.h>

class Segments
{
public:
    Segments(void) {};

    void init(uint8_t i2c_addr);

    // This custom function works somewhat like a serial.print.
    //  You can send it an array of chars (string) and it'll print
    //  the first 4 characters in the array.
    void s7sSendStringI2C(String toSend);

    // Send the clear display command (0x76)
    //  This will clear the display and reset the cursor
    void clearDisplayI2C();

    // Set the displays brightness. Should receive byte with the value
    //  to set the brightness to
    //  dimmest------------->brightest
    //     0--------127--------255
    void setBrightnessI2C(byte value);

    // Turn on any, none, or all of the decimals.
    //  The six lowest bits in the decimals parameter sets a decimal
    //  (or colon, or apostrophe) on or off. A 1 indicates on, 0 off.
    //  [MSB] (X)(X)(Apos)(Colon)(Digit 4)(Digit 3)(Digit2)(Digit1)
    void setDecimalsI2C(byte decimals);

    // Display an unsigned 8-bit value
    void write_u8(uint8_t data);

private:
    uint8_t s7sAddress = 0x00;
};

#endif // SEGMENTS_H
