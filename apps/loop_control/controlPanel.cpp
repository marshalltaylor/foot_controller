//********************************************//
#include "controlPanel.h"
#include "HardwareInterfaces.h"
#include "PanelComponents.h"
#include "globals.h"
#include "sx1509Interface.h"
#include <Arduino.h>

//---Selector---------------------------------------------------------
Selector::Selector(void)
{
    points = 10;
}

Selector::~Selector(void)
{
    delete[] thresholds;
}

// 8 bit resolution on the ADC should be fine.
void
Selector::init(uint16_t maxInput, uint16_t minInput, uint8_t pointsInput)
{
    if (pointsInput < 2)
    {
        points = 2;
    }
    else
    {
        points = pointsInput - 1; //( by (n-1) not n )
    }
    thresholds = new uint16_t[points];

    // Set up the ranges
    uint16_t stepHeight = (maxInput - minInput) / points;
    thresholds[0]       = minInput + (stepHeight / 2);
    int i;
    for (i = 1; i < points; i++)
    {
        thresholds[i] = thresholds[i - 1] + stepHeight;
    }
}

void
Selector::freshen(uint16_t msTickDelta)
{
    // Throw away input
    // Cause the interface to get the data
    hardwareInterface->readHardware();
    // Collect the data
    KnobDataObject tempObject;
    hardwareInterface->getData(&tempObject);

    uint16_t freshData = *(uint16_t *)tempObject.data;

    state = 0;
    // Seek the position
    int i;
    for (i = 0; i < points; i++)
    {
        if (freshData > thresholds[i])
        {
            state = i + 1; // It's this or higher
        }
    }
    // Check if new
    if (state != lastState)
    {
        newData   = 1;
        lastState = state;
    }
}

uint16_t
Selector::getState(void)
{
    return state;
}

//---Panel------------------------------------------------------------
ControlPanel::ControlPanel(void)
{
    red_toggle_button.setHardware(new ArduinoDigitalIn(4), 1);
    add(&red_toggle_button);
    yellow_toggle_button.setHardware(new ArduinoDigitalIn(5), 1);
    add(&yellow_toggle_button);
    left_red_button.setHardware(new ArduinoDigitalIn(2), 1);
    add(&left_red_button);
    right_red_button.setHardware(new ArduinoDigitalIn(3), 1);
    add(&right_red_button);

    red_toggle_led.setHardware(new ArduinoDigitalOut(A8), 1);
    add(&red_toggle_led);
    yellow_toggle_led.setHardware(new ArduinoDigitalOut(A9), 1);
    add(&yellow_toggle_led);
    red_led.setHardware(new ArduinoDigitalOut(A1), 1);
    add(&red_led);
    green_led.setHardware(new ArduinoDigitalOut(A2), 1);
    add(&green_led);
    yellow_led.setHardware(new ArduinoDigitalOut(A3), 1);
    add(&yellow_led);

    // Expander pins
    green_toggle_1_led.setHardware(new sx1509DigitalOut(8), 1);
    add(&green_toggle_1_led);
    green_toggle_2_led.setHardware(new sx1509DigitalOut(9), 1);
    add(&green_toggle_2_led);
    green_toggle_3_led.setHardware(new sx1509DigitalOut(10), 1);
    add(&green_toggle_3_led);
    green_toggle_4_led.setHardware(new sx1509DigitalOut(11), 1);
    add(&green_toggle_4_led);

    green_toggle_1_button.setHardware(new sx1509DigitalIn(3), 1);
    add(&green_toggle_1_button);
    green_toggle_2_button.setHardware(new sx1509DigitalIn(2), 1);
    add(&green_toggle_2_button);
    green_toggle_3_button.setHardware(new sx1509DigitalIn(0), 1);
    add(&green_toggle_3_button);
    green_toggle_4_button.setHardware(new sx1509DigitalIn(1), 1);
    add(&green_toggle_4_button);

    left_inc_button.setHardware(new sx1509DigitalIn(7), 1);
    add(&left_inc_button);
    left_dec_button.setHardware(new sx1509DigitalIn(6), 1);
    add(&left_dec_button);
    right_inc_button.setHardware(new sx1509DigitalIn(5), 1);
    add(&right_inc_button);
    right_dec_button.setHardware(new sx1509DigitalIn(4), 1);
    add(&right_dec_button);

    selector.setHardware(new ArduinoAnalogIn(A0));
    selector.init(1024, 0, 10); // With max, min, points
    add(&selector);

    state = PInit;
    held  = 0;
}

void
ControlPanel::reset(void)
{
    // Set explicit states
    // Set all LED off
    red_toggle_led.setState(LEDOFF);
    yellow_toggle_led.setState(LEDOFF);
    red_led.setState(LEDOFF);
    green_led.setState(LEDOFF);
    yellow_led.setState(LEDOFF);

    green_toggle_1_led.setState(LEDOFF);
    green_toggle_2_led.setState(LEDOFF);
    green_toggle_3_led.setState(LEDOFF);
    green_toggle_4_led.setState(LEDOFF);

    leftDisplay.init(0x71);
    rightDisplay.init(0x30);

    state = PInit;
}

static MidiMessage msg_scratch = {
    .channel = DEVICE_MIDI_CHANNEL,
    .tick    = 0,
};

void
ControlPanel::send_note_on(uint8_t key)
{
    msg_scratch.controlMask = NoteOn;
    msg_scratch.value       = key;
    msg_scratch.data        = 64;
    outMain.input(&msg_scratch, 0);
}

void
ControlPanel::send_note_off(uint8_t key)
{
    msg_scratch.controlMask = NoteOff;
    msg_scratch.value       = key;
    msg_scratch.data        = 0;
    outMain.input(&msg_scratch, 0);
}

void
ControlPanel::tickStateMachine(int msTicksDelta)
{
    freshenComponents(msTicksDelta);
    expander.update();

    bool update_left     = false;
    bool update_right    = false;
    bool update_selector = false;

    if (state == PInit)
    {
        // Load default
        left_ctr  = 0;
        right_ctr = 0;
        // trigger screen update
        update_left  = true;
        update_right = true;
        // trigger selector update
        update_selector = true;
        // Send midi clear
        // Move to new state
        state = PRun;
    }

    if (left_red_button.serviceRisingEdge())
    {
        send_note_on(0x20);
    }
    if (left_red_button.serviceFallingEdge())
    {
        send_note_off(0x20);
    }

    if (right_red_button.serviceRisingEdge())
    {
        send_note_on(0x21);
    }
    if (right_red_button.serviceFallingEdge())
    {
        send_note_off(0x21);
    }

    if (red_toggle_button.serviceRisingEdge())
    {
        red_toggle_led.setState(LEDON);
        send_note_on(0x22);
    }
    if (red_toggle_button.serviceFallingEdge())
    {
        red_toggle_led.setState(LEDOFF);
        send_note_off(0x22);
    }

    if (yellow_toggle_button.serviceRisingEdge())
    {
        yellow_toggle_led.setState(LEDON);
        send_note_on(0x23);
    }
    if (yellow_toggle_button.serviceFallingEdge())
    {
        yellow_toggle_led.setState(LEDOFF);
        send_note_off(0x23);
    }

    if (green_toggle_1_button.serviceRisingEdge())
    {
        green_toggle_1_led.setState(LEDON);
        send_note_on(0x24);
    }
    if (green_toggle_1_button.serviceFallingEdge())
    {
        green_toggle_1_led.setState(LEDOFF);
        send_note_off(0x24);
    }

    if (green_toggle_2_button.serviceRisingEdge())
    {
        green_toggle_2_led.setState(LEDON);
        send_note_on(0x25);
    }
    if (green_toggle_2_button.serviceFallingEdge())
    {
        green_toggle_2_led.setState(LEDOFF);
        send_note_off(0x25);
    }

    if (green_toggle_3_button.serviceRisingEdge())
    {
        green_toggle_3_led.setState(LEDON);
        send_note_on(0x26);
    }
    if (green_toggle_3_button.serviceFallingEdge())
    {
        green_toggle_3_led.setState(LEDOFF);
        send_note_off(0x26);
    }

    if (green_toggle_4_button.serviceRisingEdge())
    {
        green_toggle_4_led.setState(LEDON);
        send_note_on(0x27);
    }
    if (green_toggle_4_button.serviceFallingEdge())
    {
        green_toggle_4_led.setState(LEDOFF);
        send_note_off(0x27);
    }

    // Display
    if (left_inc_button.serviceRisingEdge())
    {
        update_left = true;
        left_ctr++;
        if (left_ctr >= 128)
        {
            left_ctr = 0;
        }
        controllers.setValue(0, left_ctr);
    }
    if (left_dec_button.serviceRisingEdge())
    {
        update_left = true;
        left_ctr--;
        if (left_ctr >= 128)
        {
            left_ctr = 127;
        }
        controllers.setValue(0, left_ctr);
    }

    if (right_inc_button.serviceRisingEdge())
    {
        update_right = true;
        right_ctr++;
        if (right_ctr >= 128)
        {
            right_ctr = 0;
        }
        controllers.setValue(1, right_ctr);
    }
    if (right_dec_button.serviceRisingEdge())
    {
        update_right = true;
        right_ctr--;
        if (right_ctr >= 128)
        {
            right_ctr = 127;
        }
        controllers.setValue(1, right_ctr);
    }

    if (update_left)
    {
        leftDisplay.write_u8(left_ctr);
    }
    if (update_right)
    {
        rightDisplay.write_u8(right_ctr);
    }

    if (selector.serviceChanged() || update_selector)
    {
        uint8_t value = selector.getState();
        if (value > 7)
        {
            value = 7;
        }
        if (value & 0x01)
        {
            red_led.setState(LEDON);
        }
        else
        {
            red_led.setState(LEDOFF);
        }
        if (value & 0x02)
        {
            green_led.setState(LEDON);
        }
        else
        {
            green_led.setState(LEDOFF);
        }
        if (value & 0x04)
        {
            yellow_led.setState(LEDON);
        }
        else
        {
            yellow_led.setState(LEDOFF);
        }
        controllers.setValue(2, value);
    }
}
