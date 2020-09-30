; pwmled_asm_direct.asm: Assembly PWM-LED functions
; Lab 7, ECE 266, Spring 2019
;
; This version uses direct I/O register access. It is starter code.
;
; Created by Zhao Zhang
;
 .cdecls "stdint.h","stdbool.h","inc/hw_memmap.h","driverlib/sysctl.h","driverlib/gpio.h","driverlib/pin_map.h","driverlib/timer.h","pwmbuzzer.h"

                .text

                ;field and any other declarations
WTIMER0_PERIPH   .field  SYSCTL_PERIPH_WTIMER0
WTIMER1_PERIPH   .field  SYSCTL_PERIPH_GPIOC
WTIMER0          .field  WTIMER0_BASE
PORTC            .field  GPIO_PORTC_BASE
PIN_ROUTE_1      .field  GPIO_PC4_WT0CCP0





; void buzzerPwmInit()
    ; Enable Timer 2 and C port periph
    ;SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
   ; SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);


 .asmfunc
buzzerPwmInit      PUSH    {lr}

				LDR     r0, WTIMER0_PERIPH
                BL      SysCtlPeripheralEnable

                LDR     r0, WTIMER1_PERIPH
                BL      SysCtlPeripheralEnable



; Connect pins to timer 2
   ; GPIOPinTypeTimer(PORTC, GPIO_PIN_5)
    ;GPIOPinConfigure(GPIO_PC5_WT0CCP1)

        LDR     r0, PORTC
                MOV     r1, #GPIO_PIN_4
                BL      GPIOPinTypeTimer

              LDR     r0, PIN_ROUTE_1
                BL      GPIOPinConfigure


                ;Select PWM for timer 2 sub timer A
   ; TimerConfigure(WTIMER0, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PWM))


                LDR     r0, TIMER0
                MOV     r1, TIMER_CFG_SPLIT_PAIR
                ORR     r1, #TIMER_CFG_A_PWM
                BL      TimerConfigure

                ;;Invert PWM Waveform
    ;;TimerControlLevel(WTIMER0, TIMER_B, true /* output inversion */)

                LDR     r0, WTIMER0
                MOV     r1, #TIMER_A
                MOV     r2, #true
                BL      TimerControlLevel



     			LDR     r0, TIMER0
                MOV     r1, #WTIMER_A
                MOV     r2, #100
                BL      TimerLoadSet

                LDR     r0, WTIMER0
                MOV     r1, #TIMER_A
                MOV     r2, #0
                BL      TimerMatchSet



  LDR    		 r0, WTIMER0,
                MOV     r1, #TIMER_A
                BL      TimerEnable

                POP     {pc}
                .endasmfunc

     .asmfunc
buzzerVolumePwmSet       PUSH    {lr,r1, r0}

<<<<<<< HEAD
				MOV     r3, r0
				MOV 	r2,r0
                LDR     r0, WTIMER0
                MOV     r1, #TIMER_A
                BL      TimerLoadSet

				MOV 	r2,r3
                LDR     r0, WTIMER0
                MOV     r1, #TIMER_A
                BL      TimerMatchSet
=======
; WTIMER0's B Interval Load (GPTMTBILR) register, offset 0x02C
; For PWM, It stores the PWM period
TIMER_TBILP_ptr    	.field  WTIMER0_BASE + TIMER_O_TBILR

; WTIMER0's B Match (GPTMTBMATCHR) register, offset 0x034
; For PWM, it stores the PWM pulse width for the inverted mode
TIMER_TBMATCHR_ptr 	.field  WTIMER0_BASE + TIMER_O_TBMATCHR
>>>>>>> c82e8a987012bd782c917c6c1953cbfcc0cfc764

                ADD     SP, #8
                POP     {pc}
                .endasmfunc
