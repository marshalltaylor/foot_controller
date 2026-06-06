/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "midiCommon.h"

/* References ----------------------------------------------------------------*/
// Connect to stdio
#include <stdio.h>
#define localPrintf printf

void
printItfMessage (MidiMessage *pMsg)
{
    localPrintf("--MidiMessage--\n");
    localPrintf(" ctl:  %0x02X\n", pMsg->controlMask);
    localPrintf(" chan: % 4d\n", pMsg->channel);
    localPrintf(" data: % 4d\n", pMsg->value);
    localPrintf(" data: % 4d\n", pMsg->data);
    localPrintf(" tick: %10u\n", (unsigned int)pMsg->tick);
}

//**********************************************************************//
