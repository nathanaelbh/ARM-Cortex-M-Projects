/*
 * ECE 266, Fall 2019, Lab 4
 *
 * main.c
 * Created by Zhao Zhang
 */

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "launchpad.h"
#include "buzzer.h"
#include "motion.h"

// ON/Off state type
typedef enum {On, Off} OnOff_t;

// System running state
static OnOff_t sysState = Off;

// Alarm state
static OnOff_t alarmState = Off;

// Buzzer state
static OnOff_t buzzerState = Off;

/*
 * Task 1: Play the buzzer
 */

// Callback function for playing the buzzer
void callbackBuzzerPlay(uint32_t time)                    // the scheduled time
{
    uint32_t delay = 10;

    if (alarmState == On)
    {
        assert(sysState == On);

        // Turn the buzzer on and off alternatively
        // Adjust the time values to control the sound intensity
        switch (buzzerState)
        {
        case On:
            buzzerOff();
            buzzerState = Off;
            delay = 2988;                           // off for 2988 ms
            break;

        case Off:
            buzzerOn();
            buzzerState = On;
            delay = 12;                            // on for 12 ms
            break;
        }
    }

    // schedule the next callback
    schdCallback(callbackBuzzerPlay, time + delay);  // loops the function
}

/*
 * Task 2: Check push buttons
 */

// Callback function for checking the push buttons
void callbackCheckPushButton(uint32_t time)
{
    uint32_t delay = 10;        // the default delay for the next checking



    int code = pbRead();        // read the pushbutton
    switch (code)
    {
    case 1:                     // SW1: Turn on the system and the alarm
        sysState = On;
        ledTurnOnOff(false /* red */, false /* blue */, true /* green */);  // turns green
        delay = 250;
        break;

    case 2:                     // SW2: Turn off the system and the alarm
        sysState = Off;
        alarmState = Off;
        ledTurnOnOff(false /* red */, false /* blue */, false /* green */);    // turns led off
        buzzerOff();
        delay = 250;
        break;
    }

    // schedule the next callback
    schdCallback(callbackCheckPushButton, time + delay); // loop
}


/*
 * Task 3: YOUR CODE, check the PIR motion sensor
 */
void callbackMotionDetect(uint32_t time) // call back funtion to motion detect
{
    if(sysState == On){
        if(motionDetect()){  // if motion
            uprintf("detected motion\r\n");
            alarmState = On;
            ledTurnOnOff(true /* red */, false /* blue */, false /* green */);   //turn red


        }
        else { // in no motion
            uprintf("no motion\r\n");
            alarmState = Off;
            ledTurnOnOff(false /* red */, false /* blue */, true /* green */); // turn green

        }
    }schdCallback(callbackMotionDetect,time+1000); //every second
}



/*
 * The main function
 */
int main(void)
{
    lpInit();
    buzzerInit();
    motionInit();     // define motion

    // Print out a start message
    uprintf("%s\n\r", "Lab 4 starts");

    // Schedule the first callback events
    schdCallback(callbackBuzzerPlay, 1000);        // delay time 1000
    schdCallback(callbackCheckPushButton, 1002);
    schdCallback(callbackMotionDetect,1004);

    // Turn on the green sub-LED
    ledTurnOnOff(false /* red */, false /* blue */, false /* green */);

    // Run the callback scheduler
    while (true)
    {
        schdExecute();
    }
}
