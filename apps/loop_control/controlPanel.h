// Header
#ifndef LOOPERPANEL_H_INCLUDED
#define LOOPERPANEL_H_INCLUDED

#include "HardwareInterfaces.h"
#include "Panel.h"
#include "PanelComponents.h"
#include "segments.h"
#include <Arduino.h>

enum PStates
{
    PInit,
    PRun
};

//---Selector----------------------------------------------------
class Selector : public KnobParentClass
{
public:
    Selector(void);
    ~Selector(void);
    virtual void freshen(uint16_t msTickDelta);
    uint16_t     getState(void);
    void init(uint16_t, uint16_t, uint8_t); // maxInput, minInput, number of
                                            // points

protected:
private:
    uint16_t  state;
    uint16_t  lastState;
    uint16_t *thresholds;
    uint8_t   points;
};

class ControlPanel : public Panel
{
public:
    ControlPanel(void);
    void    reset(void);
    void    tickStateMachine(int msTicksDelta);
    uint8_t getState (void)
    {
        return (uint8_t)state;
    }

private:
    void send_note_on(uint8_t key);
    void send_note_off(uint8_t key);
    // Internal Panel Components
    Button red_toggle_button;
    Button yellow_toggle_button;
    Button left_red_button;
    Button right_red_button;
    Led    red_toggle_led;
    Led    yellow_toggle_led;
    Led    red_led;
    Led    green_led;
    Led    yellow_led;

    Led    green_toggle_1_led;
    Led    green_toggle_2_led;
    Led    green_toggle_3_led;
    Led    green_toggle_4_led;
    Button green_toggle_1_button;
    Button green_toggle_2_button;
    Button green_toggle_3_button;
    Button green_toggle_4_button;
    Button left_inc_button;
    Button left_dec_button;
    Button right_inc_button;
    Button right_dec_button;

    Segments leftDisplay;
    Segments rightDisplay;

    Selector selector;

    //  ..and data
    uint8_t held;

    // State machine stuff
    PStates state;

    uint8_t left_ctr;
    uint8_t right_ctr;
    uint8_t selector_ctr;
};

#endif
