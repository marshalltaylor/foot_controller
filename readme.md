# Teensy 3.2 MIDI controller

## Hardware

 ![photo of pedal board with buttons](https://raw.githubusercontent.com/marshalltaylor/foot_controller/main/doc/hardware.png "Proto-8 Circuit in fixture")

* GPIO expanded by sx1509
* Sparkfun serial 7-segment displays
* Selector with resistor divider on analog input

## Software

Install Teensy package onto the Arduino IDE

Add packages from `/arduino_libs`

### MIDI implementation

__Channel__

Hardcoded channel 0

__Controllers__

* Left: 0 (range 0-127)
* Right: 1 (range 0-127)
* Selector: 2 (range 0-9)
