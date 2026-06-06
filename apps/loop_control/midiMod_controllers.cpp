#include <stdio.h>
// #include "globals.h"
#include "midiMod_controllers.h"
#include "midiCommon.h"

#define localPrintf printf

static bool hasTicked = false;

Controllers::Controllers(void)
{
}

void
Controllers::emitMsg(MidiMessage *pMsg)
{
    // Main output
    send(pMsg, 0);
    // Aux output
    send(pMsg, 1);
}

void
Controllers::setValue(uint8_t index, uint8_t value)
{
    if (index >= MIDI_MOD_CONTROLLERS_MAX)
    {
        return;
    }
    controllerValue[index] = value;
}

void
Controllers::setCtrlNum(uint8_t index, uint8_t ctrlNum)
{
    if (index >= MIDI_MOD_CONTROLLERS_MAX)
    {
        return;
    }
    controllerNumber[index] = ctrlNum;
}

void
Controllers::setChannel(uint8_t index, uint8_t channel)
{
    if (index >= MIDI_MOD_CONTROLLERS_MAX)
    {
        return;
    }
    controllerChannel[index] = channel;
}

void
Controllers::tick(void)
{
    if (!hasTicked)
    {
        // On first tick, don't tx anything, just load the state vars
        for (int i = 0; i < MIDI_MOD_CONTROLLERS_MAX; i++)
        {
            lastControllerValue[i] = controllerValue[i];
        }
        // Don't do this again
        hasTicked = true;
    }
    for (int i = 0; i < MIDI_MOD_CONTROLLERS_MAX; i++)
    {
        if (lastControllerValue[i] != controllerValue[i])
        {
            // send it
            MidiMessage Msg = {
                .controlMask = ControlChange,
                .channel     = controllerChannel[i],
                .value       = controllerNumber[i],
                .data        = controllerValue[i],
                .tick        = 0,
            };
            emitMsg(&Msg);
            lastControllerValue[i] = controllerValue[i];
        }
    }
}

void
Controllers::printDebug(void)
{
    localPrintf("Controllers data\n");
    for (int i = 0; i < MIDI_MOD_CONTROLLERS_MAX; i++)
    {
        localPrintf("[ctrl %d:", controllerNumber[i]);
        localPrintf("%d,", controllerValue[i]);
        localPrintf("%d] ", lastControllerValue[i]);
        if ((i + 1) % 12 == 0)
        {
            localPrintf("\n");
        }
    }
    localPrintf("\n");
}
