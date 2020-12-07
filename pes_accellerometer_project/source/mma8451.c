/*
 * mma8451.c: Contains functions to configure various modes of MMA8451 sensor (freefall,motion,tap)
 *
 * Created on: 29-Nov-2020
 *
 * Author: Chirayu Thakur
 *
 */

#include "mma8451.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "i2c.h"
#include "systick.h"
#include <stdlib.h>

//MMA8451Q Interrupts
#define MMA8451_INT1      14
#define MMA8451_INT2      15

//Self Test Mode Thresholds
#define THRESHOLD_X      181
#define THRESHOLD_Y      255
#define THRESHOLD_Z      1680

#define MODE_FREEFALL  1 //freefall detection mode
#define MODE_MOTION    0 //motion detection mode

extern uint32_t masking_state;
extern  volatile uint32_t currenttime;

int16_t acc_x,acc_y,acc_z;


int mode=MODE_MOTION;

/*
 * Initializes GPIO interrupts
 *
 * Parameters:None
 *
 * Returns: None
 *
 */
void Init_Interrupts(void)
{
	//Configure the PTA14 pin (connected to the INT1 of the MMA8451Q) for falling edge interrupts
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;		// Turn on clock to Port A module
	PORTA->PCR[MMA8451_INT1] |= (0|PORT_PCR_ISF_MASK|	// Clear the interrupt flag
			PORT_PCR_MUX(0x1)|	// PTA14 is configured as GPIO
			PORT_PCR_IRQC(0xA));	// PTA14 is configured for falling edge interrupts

	//Configure the PTA15 pin (connected to the INT2 of the MMA8451Q) for falling edge interrupts
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;		// Turn on clock to Port A module
	PORTA->PCR[MMA8451_INT2] |= (0|PORT_PCR_ISF_MASK|	// Clear the interrupt flag
			PORT_PCR_MUX(0x1)|	// PTA15 is configured as GPIO
			PORT_PCR_IRQC(0xA));	// PTA15 is configured for falling edge interrupts


	NVIC_SetPriority(PORTA_IRQn, 2);
	NVIC_ClearPendingIRQ(PORTA_IRQn);
	NVIC_EnableIRQ(PORTA_IRQn);
}

/*
 * Disables GPIO Interrupts
 *
 * Parameters:None
 *
 * Returns: None
 *
 */
void disable_interrupts(void)
{
	PORTA->PCR[MMA8451_INT1]&=PORT_PCR_IRQC(0x00); //Disable Freefall Interrupt
	PORTA->PCR[MMA8451_INT2]&=PORT_PCR_IRQC(0x00); //Disable Tap Interrupt
}

/*
 * Enables GPIO Interrupts
 *
 * Parameters:None
 *
 * Returns: None
 *
 */
void enable_interrupts(void)
{
	PORTA->PCR[MMA8451_INT1] |= (0|PORT_PCR_ISF_MASK|	// Clear the interrupt flag
			PORT_PCR_MUX(0x1)|	// PTA14 is configured as GPIO
			PORT_PCR_IRQC(0xA));	// PTA14 is configured for falling edge interrupts
	PORTA->PCR[MMA8451_INT2] |= (0|PORT_PCR_ISF_MASK|	// Clear the interrupt flag
			PORT_PCR_MUX(0x1)|	// PTA15 is configured as GPIO
			PORT_PCR_IRQC(0xA));	// PTA15 is configured for falling edge interrupts
}

/*
 * Implements self test feature of MMA8451 sensor
 *
 * Parameters:None
 *
 * Returns: True if test passed,False if test fails
 *
 */
bool self_test(void)
{
	int16_t x_ston=0,x_stoff=0;
	int16_t y_ston=0,y_stoff=0;
	int16_t z_ston=0,z_stoff=0;



	uint8_t CTRL_REG1_Data ;

	//Self Test off
	I2C_WriteRegister(MMA845x_I2C_ADDRESS,CTRL_REG1, 0x0C); //400Hz,Active Mode
	I2C_WriteRegister(MMA845x_I2C_ADDRESS,CTRL_REG1, 0x0D); //400Hz,Active Mode
	reset_timer();
	while(get_timer()<INTERVAL_20_MS); //20ms delay
	read_accelleration(ACC_X);
	x_stoff+=acc_x;
	read_accelleration(ACC_Y);
	y_stoff+=acc_y;
	read_accelleration(ACC_Z);
	z_stoff+=acc_z;

	//self Test on
	I2C_WriteRegister(MMA845x_I2C_ADDRESS,CTRL_REG1, 0x0C); //Set the device in 400 Hz ODR, Standby
	I2C_WriteRegister(MMA845x_I2C_ADDRESS,CTRL_REG2, 0x080); //Self test mode on
	CTRL_REG1_Data = I2C_ReadRegister(MMA845x_I2C_ADDRESS,CTRL_REG1);
	CTRL_REG1_Data|= 0x01;
	I2C_WriteRegister(MMA845x_I2C_ADDRESS,CTRL_REG1,CTRL_REG1_Data); //Active Mode
	reset_timer();
	while(get_timer()<INTERVAL_20_MS); //20ms delay
	read_accelleration(ACC_X);
	x_ston+=acc_x;
	read_accelleration(ACC_Y);
	y_ston+=acc_y;
	read_accelleration(ACC_Z);
	z_ston+=acc_z;


	//compare and check if measured values lie within acceptable range

   if((abs(x_ston-x_stoff)>=THRESHOLD_X) && (abs(y_ston-y_stoff)>=THRESHOLD_Y) && (abs(z_ston-z_stoff)>=THRESHOLD_Z))
   {
	   return true;
   }

    return false;

}

/*
 * Configures MMA8451 sensor for freefall Detection
 *
 * Parameters:None
 *
 * Returns: None
 *
 */
void Init_Freefall(void)
{

	mode=MODE_FREEFALL;
	uint8_t CTRL_REG1_Data ;
	I2C_WriteRegister(MMA845x_I2C_ADDRESS,CTRL_REG1, 0x0C); //Set the device in 400 Hz ODR, Standby
	I2C_WriteRegister(MMA845x_I2C_ADDRESS,FF_MT_CFG_REG,0xB8); //Configuration Register set for Freefall Detection enabling “AND” condition, OAE = 0, Enabling
	// Z and the Latch
	I2C_WriteRegister(MMA845x_I2C_ADDRESS,FT_MT_THS_REG,0x03);//Threshold Setting Value for the resulting acceleration < 0.2g
	I2C_WriteRegister(MMA845x_I2C_ADDRESS,FF_MT_COUNT_REG, 0x0A); //Set Debounce counter 25ms
	I2C_WriteRegister(MMA845x_I2C_ADDRESS,CTRL_REG4,0x04); //Enable free fall interrupt
	I2C_WriteRegister(MMA845x_I2C_ADDRESS,CTRL_REG5,0x04); //Route Interrupt to INT1
	CTRL_REG1_Data = I2C_ReadRegister(MMA845x_I2C_ADDRESS,0x2A);
	CTRL_REG1_Data|= 0x01;
	I2C_WriteRegister(MMA845x_I2C_ADDRESS,0x2A,CTRL_REG1_Data); //Set Device to Active Mode



}

/*
 * Reads acceleration values from MMA8451 sensor
 *
 * Parameters:
 *
 *   int axis: One of three axis (x,y or z) whose acceleration value needs to be read
 *
 * Returns:
 *
 */
void read_accelleration(int axis)
{
	int acc_msb;
	int acc_lsb;

	if(axis==ACC_X)
	{
		acc_msb=I2C_ReadRegister(MMA845x_I2C_ADDRESS,OUT_X_MSB_REG);
		acc_lsb=I2C_ReadRegister(MMA845x_I2C_ADDRESS,OUT_X_LSB_REG);
		acc_x=(int16_t)(acc_msb<<8|acc_lsb);
		acc_x=acc_x/4; //align to 14 bits
	}

	if(axis==ACC_Y)
	{
		acc_msb=I2C_ReadRegister(MMA845x_I2C_ADDRESS,OUT_Y_MSB_REG);
		acc_lsb=I2C_ReadRegister(MMA845x_I2C_ADDRESS,OUT_Y_LSB_REG);
		acc_y=(int16_t)(acc_msb<<8|acc_lsb);
		acc_y=acc_y/4; //align to 14 bits
	}

	if(axis==ACC_Z)
	{
		acc_msb=I2C_ReadRegister(MMA845x_I2C_ADDRESS,OUT_Z_MSB_REG);
		acc_lsb=I2C_ReadRegister(MMA845x_I2C_ADDRESS,OUT_Z_LSB_REG);
		acc_z=(int16_t)(acc_msb<<8|acc_lsb);
		acc_z=acc_z/4; //align to 14 bits
	}

}


/*
 * Configures MMA8451 sensor for motion detection
 *
 * Parameters:None
 *
 * Returns: None
 *
 */
void Init_Motion(void)
{

	mode=MODE_MOTION; // motion detection
	uint8_t CTRL_REG1_Data ;
	I2C_WriteRegister(MMA845x_I2C_ADDRESS,CTRL_REG1,0x0C);		//Set the device in 400 Hz ODR, Standby
	I2C_WriteRegister(MMA845x_I2C_ADDRESS,FF_MT_CFG_REG,0xF8); 			//Set motion Event flag in x,y direction
	I2C_WriteRegister(MMA845x_I2C_ADDRESS,FT_MT_THS_REG,0x10); 			//Set Threshold value to 16 counts
	I2C_WriteRegister(MMA845x_I2C_ADDRESS,CTRL_REG4,0x04);              //Enable Motion Interrupt
	I2C_WriteRegister(MMA845x_I2C_ADDRESS,CTRL_REG5,0x04);              //Route Interrupt to INT1
	CTRL_REG1_Data = I2C_ReadRegister(MMA845x_I2C_ADDRESS,0x2A);
	CTRL_REG1_Data|= 0x01;
	I2C_WriteRegister(MMA845x_I2C_ADDRESS,0x2A,CTRL_REG1_Data);        //Set Device to Active Mode


}


/*
 * Configures MMA8451 sensor for tap detection
 *
 * Parameters: None
 *
 * Returns: None
 *
 */
void Init_Tap(void)
{


	uint8_t CTRL_REG1_val,CTRL_REG4_val ;

	CTRL_REG1_val = I2C_ReadRegister(MMA845x_I2C_ADDRESS, CTRL_REG1);   //Save Previous state of Register
	I2C_WriteRegister(MMA845x_I2C_ADDRESS, CTRL_REG1, 0x0C);			// ODR = 400Hz, Reduced noise, Standby mode
	I2C_WriteRegister(MMA845x_I2C_ADDRESS, XYZ_DATA_CFG_REG, 0x00);		// +/-2g range -> 1g = 16384/4 = 4096 counts
	I2C_WriteRegister(MMA845x_I2C_ADDRESS, CTRL_REG2, 0x02);		    // High Resolution mode

	I2C_WriteRegister(MMA845x_I2C_ADDRESS, PULSE_CFG_REG, 0x15);     	//Enable X, Y and Z Single Pulse
	I2C_WriteRegister(MMA845x_I2C_ADDRESS, PULSE_THSX_REG, 0x20);	 	//Set X Threshold to 2.016g
	I2C_WriteRegister(MMA845x_I2C_ADDRESS, PULSE_THSY_REG, 0x20);		//Set Y Threshold to 2.016g
	I2C_WriteRegister(MMA845x_I2C_ADDRESS, PULSE_THSZ_REG, 0x2A);		//Set Z Threshold to 2.646g
	I2C_WriteRegister(MMA845x_I2C_ADDRESS, PULSE_TMLT_REG, 0x28);		//Set Time Limit for Tap Detection to 25 ms
	I2C_WriteRegister(MMA845x_I2C_ADDRESS, PULSE_LTCY_REG, 0x28);		//Set Latency Time to 50 ms
	CTRL_REG4_val=I2C_ReadRegister(MMA845x_I2C_ADDRESS, CTRL_REG4);
	I2C_WriteRegister(MMA845x_I2C_ADDRESS, CTRL_REG4, CTRL_REG4_val|0x08);			//Pulse detection interrupt enabled



	CTRL_REG1_val |= 0x01;
	I2C_WriteRegister(MMA845x_I2C_ADDRESS, CTRL_REG1, CTRL_REG1_val);    //Set Device to Active Mode
}


/*
 * Resets registers  of MMA8451 sensor
 *
 * Parameters: None
 *
 * Returns: None
 *
 */
void Reset_Registers(void)
{
	uint8_t reg_val = 0;

	I2C_WriteRegister(MMA845x_I2C_ADDRESS, CTRL_REG2, 0x40);		// Reset all registers to POR values

	do		// Wait for the RST bit to clear
	{
		reg_val = I2C_ReadRegister(MMA845x_I2C_ADDRESS, CTRL_REG2) & 0x40;
	} 	while (reg_val);

}

/*
 * Interrupt Handler for Port A GPIO Interrupts
 *
 * Parameters: None
 *
 * Returns: None
 *
 */
void PORTA_IRQHandler()
{
	NVIC_ClearPendingIRQ(PORTA_IRQn);

	if(PORTA->ISFR & MASK(MMA8451_INT1))
	{
		PORTA->PCR[MMA8451_INT1]|=PORT_PCR_ISF_MASK;
		if(mode==MODE_FREEFALL)
		{

			disable_interrupts();
			I2C_ReadRegister(MMA845x_I2C_ADDRESS,FF_MT_SRC_REG); //read latch
			detect_freefall();
		}

		else{

			detect_motion();
		}

	}

	else if(PORTA->ISFR & MASK(MMA8451_INT2))
		{
			PORTA->PCR[MMA8451_INT2]|=PORT_PCR_ISF_MASK;
			disable_interrupts();
			detect_tap();

		}

}
