/*
 * state_machine.c: Contains functions that implement state machine
 *
 * Created on: 02-Dec-2020
 *
 *Author: Chirayu Thakur
 */

#include "state_machine.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "systick.h"
#define BLINK_ON 1 //LED on during blink sequence
#define BLINK_OFF 0 //LED off during blink sequence
#define STEP 20  //To give 10 second Blinking Sequence
#define SCALE 30 //Divide by Scaling factor before assigning colour values
extern int16_t acc_x,acc_y,acc_z;
volatile uint32_t currenttime;
uint32_t masking_state;
const RGB_Value_t Magenta={255,0,255};
const RGB_Value_t Red={255,0,0};
const RGB_Value_t Green={0,255,0};
const RGB_Value_t White={255,255,255};
const RGB_Value_t Off={0,0,0};
RGB_Value_t Motion;

typedef enum
{
	STATE_CONFIG_MOTION,
	STATE_MOTION,
	STATE_CONFIG_FREEFALL,
	STATE_FREEFALL



}state_t;

typedef enum
{
	TAP,
	MOTION,
	FREEFALL,
	TIMER_EXPIRED,
	IDLE

}event_t;

struct {
	state_t state;
	event_t event;
}MMA8451_ACC={
	.state=STATE_CONFIG_MOTION,
	.event=TIMER_EXPIRED
};

/*
 * Produces output on RGB LED depending upon measured acceleration
 * in x,y,z direction
 *
 * Parameters: None
 *
 * Returns: None
 */
void control_led(void)
{
	I2C_ReadRegister(MMA845x_I2C_ADDRESS,FF_MT_SRC_REG);
	read_accelleration(ACC_X);
	read_accelleration(ACC_Y);
	read_accelleration(ACC_Z);
	Motion.RED_Value=acc_x/SCALE;
	Motion.GREEN_Value=acc_y/SCALE;
	Motion.BLUE_Value=acc_z/SCALE;
	set_colour(Motion);
}

/*
 * Produces 10s Blinking pattern on RGB LED upon detecting freefall
 *
 * Parameters: None
 *
 * Returns: True if blinking pattern is complete, false otherwise
 */
bool freefall_detect(void)
{
	static int count=0;
	static bool state=BLINK_ON;
	static bool reset=0;
	if(reset==0)
	{
		reset_timer();
		reset=1;
	}
	if(count<STEP)
	{
		if(get_timer()<INTERVAL_500_MS)
		{   if(state==BLINK_ON)
		   {
			set_colour(Magenta);

		    }

		  else{

			set_colour(Off);

		    }

		return false;
		}

		else{

			reset_timer();
			if(state==BLINK_ON)
			{
				state=BLINK_OFF; //LED Off
			}

			else{
				state=BLINK_ON; //LED On
			    }

			count++;  //Increment count
			return false;
		  }
	}

	count=0;
	state=1;
	return true;  //blink sequence complete
}

/*
 * Produces output on RGB LED for 2s upon detecting tap
 *
 * Parameters: None
 *
 * Returns: True if 2 seconds are complete,false otherwise
 */
bool tap_detect(void)
{
	static bool reset = 1;
	if(reset==1)
	{
		reset_timer();
		reset=0;
	}

	if(get_timer()<INTERVAL_2000_MS)
	{
		if(MMA8451_ACC.state==STATE_MOTION)
		{
		set_colour(Magenta);

		}

		else if(MMA8451_ACC.state==STATE_FREEFALL)
		{
			set_colour(White);

		}

		return false;

	}

    reset=1;
	set_colour(Off);
	return true;

}

/*
 * Finite state machine which responds to interrupts from sensor
 *
 * Parameters: None
 *
 * Returns: None
 */
void fsm(void)
{


	switch(MMA8451_ACC.state)
	{
	case STATE_CONFIG_MOTION:

		//PRINTF("Config Motion\r\n");

		if(MMA8451_ACC.event==TIMER_EXPIRED)
		{
			masking_state=__get_PRIMASK();
			__disable_irq();
			MMA8451_ACC.state=STATE_MOTION;
			Reset_Registers();
			Init_Motion();
			Init_Tap();
			__set_PRIMASK(masking_state);
            enable_interrupts();
			set_colour(Off);
        }

		break;

	case STATE_MOTION:

		//PRINTF(" Motion\r\n");

		if(MMA8451_ACC.event==MOTION)
		{

			//for motion detection
			masking_state=__get_PRIMASK();
			__disable_irq();
			control_led();
			__set_PRIMASK(masking_state);


		}

		else if(MMA8451_ACC.event==TAP)
		{


			if(tap_detect())
			{

				MMA8451_ACC.event=IDLE;
				MMA8451_ACC.state=STATE_CONFIG_FREEFALL;
				MMA8451_ACC.event=TIMER_EXPIRED;
				enable_interrupts();

            }
		}




		break;

	case STATE_CONFIG_FREEFALL:

		//PRINTF("Config freefall\r\n");

		if(MMA8451_ACC.event==TIMER_EXPIRED)
		{

			masking_state=__get_PRIMASK();
			__disable_irq();
			Reset_Registers();
			Init_Freefall();
			Init_Tap();
			MMA8451_ACC.state=STATE_FREEFALL;
			__set_PRIMASK(masking_state);
			set_colour(Off);

		}

		break;

	case STATE_FREEFALL:

		//PRINTF("Freefall\r\n");

		if(MMA8451_ACC.event==FREEFALL)
		{

			if(freefall_detect())
			{
				MMA8451_ACC.event=IDLE;
				enable_interrupts();
			}

         }

		else if(MMA8451_ACC.event==TAP)
		{
			if(tap_detect())
			{

				MMA8451_ACC.state=STATE_CONFIG_MOTION;
				MMA8451_ACC.event=TIMER_EXPIRED;

			}

		}



		break;

	default:
		break;

	}

}

/*
 * Schedules motion event
 *
 * Parameters: None
 *
 * Returns: None
 */
void detect_motion(void)
{
	MMA8451_ACC.event=MOTION;
}

/*
 * Schedules freefall event
 *
 * Parameters: None
 *
 * Returns: None
 */
void detect_freefall(void)
{
	MMA8451_ACC.event=FREEFALL;
}

/*
 * Schedules tap event
 *
 * Parameters: None
 *
 * Returns: None
 */
void detect_tap(void)
{
	MMA8451_ACC.event=TAP;
}
