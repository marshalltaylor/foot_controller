#include "Arduino.h"
#include <stdio.h>
#include "globals.h"
extern void globalsInit(void);

//**Panel State Machine***********************//
#include "controlPanel.h"
ControlPanel controls;

uint16_t debugCounter = 0;

	
// -----------------------------------------------------------------------------
void setup()
{
	Serial.begin(115200);
	printf("App:loop_control\r\n");

	expander.init(0x3E); // initalizes Wire

	//Go to fresh state
	controls.reset();

	expander.update();

	globalsInit();
	
}

#define ESTIMATE_WORKLOAD_MS 1
#define DESIRED_PERIOD_MS 3

int second_counter = 0;

void loop()
{
	//**Process the panel and state machine***********//  
	controls.tickStateMachine(DESIRED_PERIOD_MS);
	controllers.tick();
	
	//Delay the loop
	delay(DESIRED_PERIOD_MS - ESTIMATE_WORKLOAD_MS);
	
	//Increment a counter for debug stuff
	debugCounter = debugCounter + DESIRED_PERIOD_MS;
	//printf("t=%d\r\n", debugCounter); 
	if( debugCounter > 1000 )
	{
		//Do debug stuff
		//printf("State: %d\r\n", controls.getState()); 
		debugCounter = 0;
		second_counter++;
		printf("s=%d\r\n", second_counter);
	}
	
}
