#include "Arduino.h"
#include "globals.h"

//**Panel State Machine***********************//
#include "controlPanel.h"
ControlPanel controls;

uint16_t debugCounter = 0;

	
// -----------------------------------------------------------------------------
void setup()
{
	Serial.begin(115200);
	Serial.println("Program Started");

	expander.init(0x3E); // initalizes Wire

	//Go to fresh state
	controls.reset();

	expander.update();
	
}

//Set LOOP_DELAY to length of time between ticks of everything (in ms)
#define LOOP_DELAY 5

void loop()
{
	//**Process the panel and state machine***********//  
	controls.tickStateMachine(LOOP_DELAY);
	
	//Delay the loop
	delay(LOOP_DELAY);
	
	//Increment a counter for debug stuff
	debugCounter = debugCounter + LOOP_DELAY;
	if( debugCounter > 10000 )
	{
		//Do debug stuff
		Serial.print("State: ");
		Serial.println(controls.getState()); 
		//Serial.print("Free ram: ");
		//Serial.println(freeRam());
		debugCounter = 0;
	}
	
}
