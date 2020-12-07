/*
 * i2c.h: Contains Functions to perform communication using I2C protocol
 *
 * Created on: 28-Nov-2020
 *
 * Author: Chirayu Thakur
 *
 * Code from Dean Chapter 8 as well as from the nxp forum "MMA8451Q -Single Tap Detection Bare metal example project"
 * was used as reference
 *
 */


//function prototypes
void I2C_WriteRegister(unsigned char u8SlaveAddress, unsigned char u8RegisterAddress, char u8Data);
unsigned char I2C_ReadRegister(unsigned char u8SlaveAddress, unsigned char u8RegisterAddress);
void I2C_init(void);

