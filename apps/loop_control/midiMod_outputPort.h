#ifndef OUTPUTPORT_H
#define OUTPUTPORT_H
#include <MIDI.h>
#include "midiMod.h"

#define MIDI_PORT_TYPE \
    MIDI_NAMESPACE::MidiInterface<MIDI_NAMESPACE::SerialMIDI<HardwareSerial>>
class OutputPort : public MidiModule
{
public:
    OutputPort(void);
    void setMidiInterface(MIDI_PORT_TYPE *itf);
    void input(MidiMessage *pMsg, uint8_t inPort);

private:
    MIDI_PORT_TYPE *midiItf;
};

#endif
