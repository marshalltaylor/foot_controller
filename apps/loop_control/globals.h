#ifndef GLOBALS_H
#define GLOBALS_H

#define DEVICE_MIDI_CHANNEL 7

#include "sx1509Panel_hardware.h"
extern sx1509Hardware expander;

#include "midiMod_outputPort.h"
extern OutputPort outMain;

#include "midiMod_controllers.h"
extern Controllers controllers;

#endif // GLOBALS_H
