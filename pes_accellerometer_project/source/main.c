/*
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    pes_accellerometer_project.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "i2c.h"
#include "mma8451.h"
#include "tpm.h"
#include "state_machine.h"
#include "systick.h"



extern int16_t acc_x,acc_y,acc_z;
extern const RGB_Value_t Red;
extern const RGB_Value_t Green;
extern const RGB_Value_t Off;



/*
 * Main function for reading measurements from MMA8451 sensor
 *
 * Parameters:None
 *
 * Returns: None
 *
 */
int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    I2C_init(); //initialize I2C
    init_led_tpm(); //initialize PWM
    Init_SysTick(); //initialize Systick Timer
    Init_Interrupts(); //Initialize GPIO Interrupts
    Reset_Registers(); //Reset MMA8451 registers

#ifdef DEBUG

    if(self_test()==1)
    {
    	//Test passed
    	set_colour(Green);
    	reset_timer();
    	while(get_timer()<INTERVAL_2000_MS); //2s delay
    	set_colour(Off);

    }

    else{

    	//Test Failed
    	set_colour(Red);
    	reset_timer();
    	while(get_timer()<INTERVAL_2000_MS); //2s delay
    	set_colour(Off);

    }

#endif




    while(1)
    {

      fsm(); //run state machine

    }

    return 0 ;
}
