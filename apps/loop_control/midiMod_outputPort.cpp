#include "midiCommon.h"
#include <MIDI.h>
#include "midiMod_outputPort.h"

OutputPort::OutputPort(void)
{
    midiItf = NULL;
}

void
OutputPort::setMidiInterface(MIDI_PORT_TYPE *itf)
{
    midiItf = itf;
}

void
OutputPort::input(MidiMessage *pMsg, uint8_t inPort)
{
    dPrintf("OutputPort: input()\r\n");
    dPrintNoteStr(pMsg, inPort);
    dPrintf("\r\n");
    // printItfMessage(pMsg);
    // Module only accepts 1 input
    if (inPort != 0)
    {
        return;
    }
    if (midiItf != NULL)
    {
        // Is fairly stupid, just send it out
        midiItf->send((midi::MidiType)pMsg->controlMask,
                      pMsg->value,
                      pMsg->data,
                      pMsg->channel);
    }
}
