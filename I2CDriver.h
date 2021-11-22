/*
 *  ===================================================================================
                    ______                _                _    _
                    | ___ \              | |              | |  (_)
                    | |_/ /  __ _  _ __  | |_   __ _  ___ | |_  _   ___
                    |  __/  / _` || '_ \ | __| / _` |/ __|| __|| | / __|
                    | |    | (_| || | | || |_ | (_| |\__ \| |_ | || (__
                    \_|     \__,_||_| |_| \__| \__,_||___/ \__||_| \___|

 *  ===================================================================================
 *  UVA ECE Capstone Project
 *  Team: 5 Guys One Capstone Project / Pantastic / The Pantastic 5
 *  Members: Tyler Hendricks, Thomas Forrester, Noal Zyglowicz, Andrew Tam, Kai Wong
 */

/*
 * I2CDriver.h
 *
 *  Created on: Nov 9, 2021
 *      Author: Tyler Hendricks
 *      Adapted From KienLTB to be compatible with MLX90614 SmBus
 */

#ifndef I2CDRIVER_H_
#define I2CDRIVER_H_

/********************************************************************************
 * ORIGINAL FILE INFORMATION
Module      : I2C_SW
Author      : 05/04/2015, by KienLTb - https://kienltb.wordpress.com/
Description : I2C software using bit-banging.
********************************************************************************/

/*-----------------------------------------------------------------------------*/
/* Macro definitions  */
/*-----------------------------------------------------------------------------*/
#define ACK 0x00
#define NACK 0x01
/*-----------------------------------------------------------------------------*/
/* Local Macro definitions */
/*-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------*/
/* Function prototypes  */
/*-----------------------------------------------------------------------------*/

//NOTE: Need custom Read_SCL(), Read_SDA(), Clear_SCL(), Clear_SDA() to compatible Hardware.

void I2C_Delay(void);

unsigned char Read_SCL(void); // Set SCL as input and return current level of line, 0 or 1, nomal is 1 because pullup by res
unsigned char Read_SDA(void); // Set SDA as input and return current level of line, 0 or 1, nomal is 0 because pull by res

void Clear_SCL(void); // Actively drive SCL signal Low
void Clear_SDA(void); // Actively drive SDA signal Low

void I2C_Init(void);
void I2C_Start(void);
void I2C_Stop(void);

void I2C_Writebit(unsigned char bit);
unsigned char I2C_Readbit(void);

void I2C_WriteByte(unsigned char Data);
unsigned char I2C_ReadByte(void);

void I2C_WriteData(unsigned char *Data, unsigned char DeviceAddr, unsigned char Register, unsigned char nLength);
void I2C_ReadData(unsigned char *Buff, unsigned char DeviceAddr, unsigned char Register, unsigned char nLength);
double ReadIR(void);


#endif /* I2CDRIVER_H_ */
