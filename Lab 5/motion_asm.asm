                .cdecls "stdint.h","stdbool.h","inc/hw_memmap.h","driverlib/pin_map.h","driverlib/sysctl.h","driverlib/gpio.h","motion.h", "buzzer.h"

                .text
                .global motionDetect

; GPIO peripheral, port base and pin mask value for the buzzer
; The buzzer is assumed to be connected to J17, and then the buzzer's
; signal pin is connected to PC4.
MOTION_PORT     .field  GPIO_PORTC_BASE
MOTION_PIN      .equ    GPIO_PIN_4


motionDetect    PUSH    {LR}

                LDR     r0, MOTION_PORT
                MOV     r1, #MOTION_PIN
				MOV     r2,#MOTION_PIN
                BL      GPIOPinRead

                POP     {PC}

