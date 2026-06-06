#include "globals.h"

sx1509Hardware expander;

OutputPort outMain;

Controllers controllers;

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

void
globalsInit (void)
{
    MIDI.begin(MIDI_CHANNEL_OMNI);

    // MIDI.setHandleClock(handleClock1);
    // MIDI.setHandleStart(handleStart1);
    // MIDI.setHandleContinue(handleContinue1);
    // MIDI.setHandleStop(handleStop1);
    // MIDI.setHandleNoteOn(handleNoteOn1);
    // MIDI.setHandleNoteOff(handleNoteOff1);
    // MIDI.setHandleControlChange(handleControlChange1);
    // MIDI.setHandlePitchBend(handlePitchBend1);
    // Thru option:
    MIDI.turnThruOn();

    // Wrap midi interfaces in midi modules
    outMain.setMidiInterface(&MIDI);
    outMain.setDebug(true);

    //// Route midi modules (midi graph!)
    // splitChannels.setOutput(0, &legacyAdapter, 0);
    // splitChannels.setOutput(1, &outputMixer, 0);
    // splitChannels.setOutput(2, &arp, 1);
    // splitChannels.setOutput(3, &auxCtrl, 0);
    //
    // outputMixer.setOutput(0, &arp, 0);
    // arp.setOutput(0, &outMain, 0);
    // arp.setOutput(1, &outAux, 0);

    controllers.setCtrlNum(0, 20); // Start of undefined controllers
    controllers.setChannel(0, DEVICE_MIDI_CHANNEL);
    controllers.setCtrlNum(1, 21);
    controllers.setChannel(1, DEVICE_MIDI_CHANNEL);
    controllers.setCtrlNum(2, 22);
    controllers.setChannel(2, DEVICE_MIDI_CHANNEL);

    controllers.setOutput(0, &outMain, 0);
}
