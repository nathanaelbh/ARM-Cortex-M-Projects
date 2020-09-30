/*
 * ras.c
 *
 *  Created on: Mar 11, 2019
 *      Author: 
 */

#include <stdint.h>
#include <stdbool.h>
#include <driverlib/sysctl.h>
#include <inc/hw_memmap.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include "launchpad.h"
#include <driverlib/adc.h>
#include "ras.h"

void ADCInit()
{

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0))
    { }

    ADCSequenceConfigure(ADC0_BASE, 0 ,ADC_TRIGGER_PROCESSOR, 0);

    ADCSequenceStepConfigure(ADC0_BASE,0,0,ADC_CTL_CH7 );
    ADCSequenceStepConfigure(ADC0_BASE,0, 1 ,ADC_CTL_IE| ADC_CTL_END| ADC_CTL_CH6);



    ADCSequenceEnable(ADC0_BASE, 0);

}
