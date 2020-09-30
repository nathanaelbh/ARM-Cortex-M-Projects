;ADC operation function(s)

                    .cdecls "stdint.h", "stdbool.h", "stdio.h", "inc/hw_memmap.h", "driverlib/pin_map.h", "driverlib/gpio.h", "driverlib/sysctl.h", "driverlib/adc.h", "launchpad.h", "ras.h"
					.text
					.global ADCRead
RAS_PORT			.field ADC0_BASE

;to initialize the adc sensor

ADCRead 	PUSH{LR, r0}
						LDR r0, RAS_PORT 		;loads the RAS_PORT in r0 , r0 = Ras_Port
						MOV r1, #0				;stores 0 into r1, r1 = 0
						BL ADCProcessorTrigger	;Calls subroutine

						;loop begins
Loop              		LDR   r0, RAS_PORT		;loads RAS_PORT into r0
                        MOV          r1, #0		;stores 0 in r1, r1= 0
                        MOV          r2, #false ;stores false into r2, r2 = false
                        BL           ADCIntStatus ;Calls subroutine
                        CMP         r0, #0		 ;Compares the data in r0 witht 0
                        BEQ 		Loop		;Branches if they are equal

                        LDR				r0, RAS_PORT	;loads RAS_PORT into r0
                        MOV   			r1, #0			;Stores 0 into r1
                        POP				{R2}			;Takes r2 out of stack
                        BL				ADCSequenceDataGet	;

                        LDR				r0, RAS_PORT
                        MOV  			r1, #0
                        BL				ADCIntClear


                       POP {PC}


















;/*
 ;* ras_asm.asm
 ;*
; *  Created on: Oct 22, 2019
; *      Author: Nathanael Bhooshi
; */

;.cdecls "stdint.h","stdbool.h","inc/hw_memmap.h","stdio.h","driverlib/pin_map.h","driverlib/sysctl.h","driverlib/gpio.h","driverlib/adc.h","ras.h", "launchpad.h"
.cdecls "stdint.h", "stdbool.h", "stdio.h", "inc/hw_memmap.h", "driverlib/pin_map.h", "driverlib/gpio.h", "driverlib/sysctl.h", "driverlib/adc.h", "launchpad.h", "ras.h"
                .text
                .global rasRead

ADC_PORT	.field    ADC0_BASE


rasRead		PUSH {r0,LR}
			LDR 	r0, ADC_PORT
			MOV		r1, #0
			BL		ADCProcessorTrigger

repeat
			LDR 	r0, ADC_PORT
			MOV		r1, #0
			MOV		r2, #0

			BL 		ADCIntStatus
			CMP 	r0, #0

			BEQ 	repeat

			LDR 	r0, ADC_PORT
			MOV 	r1, #0
			POP  	{r2}

			BL 		ADCSequenceDataGet

			LDR		r0, ADC_PORT
			MOV 	r1, #0

			BL		ADCIntClear

			POP 	{PC}








