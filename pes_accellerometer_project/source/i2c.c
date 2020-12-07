/*
 * i2c.c: Contains Functions to perform communication using I2C protocol
 *
 * Created on: 28-Nov-2020
 *
 * Author: Chirayu Thakur
 *
 * Code from Dean Chapter 8 as well as from the nxp forum "MMA8451Q -Single Tap Detection Bare metal example project"
 * was used as reference
 *
 */


#include <MKL25Z4.h>
#include "i2c.h"
#include "systick.h"

#define I2C_SCL 24
#define I2C_SDA 25
#define I2C_TIMING 0x14
#define I2C_DisableAck()       I2C0->C1 |= I2C_C1_TXAK_MASK
#define I2C_EnableAck()        I2C0->C1 &= ~I2C_C1_TXAK_MASK
#define I2C_RepeatedStart()    I2C0->C1 |= I2C_C1_RSTA_MASK
#define I2C_EnterRxMode()      I2C0->C1 &= ~I2C_C1_TX_MASK
#define I2C_write_byte(data)   I2C0->D = data
#define DELAY_COUNT            50

#define I2C_Start()            I2C0->C1 |= I2C_C1_TX_MASK;\
                               I2C0->C1 |= I2C_C1_MST_MASK

#define I2C_Stop()             I2C0->C1 &= ~I2C_C1_MST_MASK;\
                               I2C0->C1 &= ~I2C_C1_TX_MASK

#define I2C_Wait()             while((I2C0->S & I2C_S_IICIF_MASK)==0) {} \
                               I2C0->S |= I2C_S_IICIF_MASK;

//function prototype
void Delay(int number);

/*
 * Initializes I2C Peripheral
 *
 * Parameters:None
 *
 * Returns: None
 *
 */
void I2C_init(void)
{
	//I2C0 module initialization
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;		// Turn on clock to I2C0 module
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;		// Turn on clock to Port E module
	PORTE->PCR[I2C_SCL] = PORT_PCR_MUX(5);			// PTE24 pin is I2C0 SCL line
	PORTE->PCR[I2C_SDA] = PORT_PCR_MUX(5);			// PTE25 pin is I2C0 SDA line
	I2C0->F  = I2C_TIMING; 						// SDA hold time = 2.125us, SCL start hold time = 4.25us, SCL stop hold time = 5.125us *
	I2C0->C1 = I2C_C1_IICEN_MASK;    		// Enable I2C0 module
}

/*
 * Writes data to register of mma8451 sensor
 *
 * Parameters:
 *
 *        unsigned char u8SlaveAddress: Address of mma8451 sensor
 *        unsigned char u8RegisterAddress: Address of register
 *        char u8Data: Data to be written
 *
 *
 * Returns: None
 *
 */
void I2C_WriteRegister(unsigned char u8SlaveAddress, unsigned char u8RegisterAddress,  char u8Data)
{
	I2C_Start();
	I2C0->D = u8SlaveAddress << 1;									// Send I2C device address with W/R bit = 0
	I2C_Wait();

	I2C0->D = u8RegisterAddress;										// Send register address
	I2C_Wait();

	I2C0->D = u8Data;												// Send the data
	I2C_Wait();

	I2C_Stop();

    Delay(DELAY_COUNT);                                                     //Small Delay between I2C transfers
}

/*
 * Reads data from mma8451 sensor
 *
 * Parameters:
 *
 *        unsigned char u8SlaveAddress: Address of mma8451 sensor
 *        unsigned char u8RegisterAddress: Address of register to be read
 *
 *
 * Returns: None
 */
unsigned char I2C_ReadRegister(unsigned char u8SlaveAddress, unsigned char u8RegisterAddress)
{
	unsigned char result;

	I2C_Start();
	I2C0->D = u8SlaveAddress << 1;									// Send I2C device address with W/R bit = 0
	I2C_Wait();

	I2C0->D = u8RegisterAddress;										// Send register address
	I2C_Wait();

	I2C_RepeatedStart();

	I2C0->D = (u8SlaveAddress << 1) | 0x01;							// Send I2C device address this time with W/R bit = 1
	I2C_Wait();

	I2C_EnterRxMode();
	I2C_DisableAck();

	result = I2C0->D;
	I2C_Wait();
	I2C_Stop();
	result = I2C0->D;
	Delay(DELAY_COUNT);                                                   //Small Delay between I2C transfers
	return result;
}

/*
 * Produces small delay between I2C transfers
 *
 * Parameters:
 *
 *           int number: Delay in ms
 *
 * Returns: None
 */

void Delay(int number)
{
	int cnt;
	for(cnt=0; cnt<number; cnt++)
	{
		asm("nop");
	};
}
