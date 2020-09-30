/*
 * ECE 266 Lab 2, Fall 2019
 * main.c
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <driverlib/sysctl.h>
#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <inc/hw_i2c.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/i2c.h>
#include "launchpad.h"
#include "seg7.h"

// 7-segment coding table. See https://en.wikipedia.org/wiki/Seven-segment_display. The segments
// are named as A, B, C, D, E, F, G. In this coding table, segments A-G are mapped to bits 0-7.
// Bit 7 is not used in the coding. This display uses active high signal, in which '1' turns ON a
// segment, and '0' turns OFF a segment.
static uint8_t seg7Coding[10] = {
        0b00111111,         // digit 0
        0b00000110,         // digit 1
        0b01011011,         // digit 2
        0b01001111,         // digit 3
        0b01100110,         // 4
        0b01101101,         //5
        0b01111101,         //6
        0b00000111,         //7
        0b01111111,         //8
        0b01100111,         //9
        // MORE CODINGS
        };

// The colon status: if colon == 0b10000000, then the colon is on,
// otherwise it is off.
static uint8_t colon = 5;

   int counter0 = 0;
   int counter1 = 0;
   int counter2 = 0;
   int counter3 = 0;
// Update the clock display
void clockUpdate(uint32_t time)                     // pointer to a 4-byte array
{
    uint8_t code[4];            // The 7-segment code for the four clock digits


    // Display 01:23 on the 7-segment displays
    // The colon ':' will flash on and off every 0.5 seconds

    code[0] = seg7Coding[counter0] + colon;
    code[1] = seg7Coding[counter1] + colon;
    code[2] = seg7Coding[counter2] + colon;
    code[3] = seg7Coding[counter3] + colon;
    seg7Update(code);

    // Calculate the display digits and colon setting for the next update
    if (colon == 0b00000000)
    {
        colon = 0b10000000;
        if (counter0 <= 9) {
                   counter0++;
                   if (counter0 > 9) {
                       counter0 = 0;
                       counter1++;
                           if(counter1 >= 6){
                               counter1 = 0;
                               counter2++;
                                   if(counter2 > 9){
                                       counter2 = 0;
                                       counter3++;
                                           if(counter3 == 6){
                                               counter3 = 0;
                                           }
                                   }


                           }

                   }
               }
    }
    else
    {
        colon = 0b00000000;
    }

    // Call back after 1 second
    schdCallback(clockUpdate, time + 500); // controls the colon
}

void
checkPushButton(uint32_t time)
{
    // Read the pushbutton state; see pbRead() in launchpad.h
    int code = pbRead();
    uint32_t delay = 10;

    switch (code) {
    case 1:
        // Switch to next color setting, with wrap-around
        //colorChoice = (colorChoice + 1) % LED_COLOR_NUM;
        counter2++;
                if(counter2 == 10){

                    counter3++;
                    counter2 = 0;
                    //printf("%d", counter2);
                }
                if(counter3 == 6){
                    counter0 = 0;
                    counter1 = 0;
                    counter2 = 0;
                    counter3 = 0;
                }




        // Use an inertia for soft de-bouncing
        delay = 250;
        break;

    case 2:
        // Pring out a message
        // See uprintf() in launchpad.h
        uprintf("%s\n\r", "SW2 is pushed");


        counter0++;
                if(counter0 == 10){
                    counter1++;
                    counter0 = 0;
                }
                if(counter1 == 6){
                    counter2++;
                    counter1 = 0;
                }
                if(counter2 == 10){
                    counter3++;
                    counter2 =0;
                }
                if(counter3 == 6){
                                    counter0 = 0;
                                    counter1 = 0;
                                    counter2 = 0;
                                    counter3 = 0;
                                }


        // Use an inertia for soft de-bouncing
        delay = 250;
        break;
    }

    schdCallback(checkPushButton, time + delay);
}

int main(void)
{
    lpInit();
    seg7Init();

    uprintf("%s\n\r", "Lab 2: Wall clock");

    // Schedule the first callback events for LED flashing and push button checking.
    // Those trigger callback chains. The time unit is millisecond.
    schdCallback(clockUpdate, 1000); // how fast the clock moves 1000
    checkPushButton(10);

    // Loop forever
    while (true)
    {
        schdExecute();
    }
}
