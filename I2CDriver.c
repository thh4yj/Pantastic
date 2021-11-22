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
 * I2CDriver.c
 *
 *  Created on: Nov 9, 2021
 *      Author: Tyler Hendricks
 *
 *      To avoid using prohibited names, the following I2C names have been renamed:
 *      SCL is named SCL2
 *      SDA is named SDA2
 *
 *      This is the I2C driver using bit bang instead of the provided TI I2C driver code.
 *      This is required because the TI code is not adaptable to work with SmBus for the MLX90614
 */

#include <I2CDriver.h>
#include <ti/drivers/GPIO.h>
#include "ti_drivers_config.h"
#include <unistd.h>


/*--------------------------------------------------------------------------------
Function    :   I2C_Delay
Purpose     :   Use an assembly NOP to do nothing for 80 clock cycles to generate a delay.
Parameters  :   None
Return      :   None
Calls       :   Called multiple times per I2C cycle to generate delays between clock cycles.
                Is allowed to be called externally for generic delays such as for the piezzo if needed.
--------------------------------------------------------------------------------*/
void I2C_Delay(void){
    int i;
    for(i = 0; i < 80; i++){
        __asm("nop \n"); //generate assembly NOP.
    }

}

 /*--------------------------------------------------------------------------------
Function    :   Read_SCL
Purpose     :   Set SCL as input and return current Logic level of SCL (0 or 1),
                normal is 1 because pull-up by resistor. Should never be set by the slave so
                this function will set the SCL to 1.
Parameters  :   None
Return      :   Logic level of SCL pin
Calls       :   Called multiple times per I2C cycle. Should not be called outside of I2C context.
--------------------------------------------------------------------------------*/
unsigned char Read_SCL(void){
    //Set SCL as gpio input, pull up resistor, no interrupts.
    GPIO_setConfig(SCL2, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_NONE);
    //return the value of the clock.
    return GPIO_read(SCL2);

}

/*--------------------------------------------------------------------------------
Function    :   Read_SDA
Purpose     :   Set SDA as input and return current Logic level of SDA (0 or 1),
                normal is 1 because pull-up by resistor. Serves to set the SDA to 1, or allow the slave to
                set it and the master to read the result.
Parameters  :   None
Return      :   Logic level of SDA pin
Calls       :   Called multiple times per I2C cycle. Should not be called outside of I2C context.
--------------------------------------------------------------------------------*/
unsigned char Read_SDA(void){
  //Set SDA as gpio input, pull up resistor, no interrupts.
  GPIO_setConfig(SDA2, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_NONE);
  //return the value of the SDA.
  return GPIO_read(SDA2);
}

/*--------------------------------------------------------------------------------
Function    :   Clear_SCL
Purpose     :   Set SCL as Output, which serves to set the SCL as logic low. The pin is also explicitly set to low.
Parameters  :   None
Return      :   None
Calls       :   Called multiple times per I2C cycle. Should not be called outside of I2C context.
--------------------------------------------------------------------------------*/
void Clear_SCL(void){
  //set the SCL as output, low
  GPIO_setConfig(SCL2, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
}
/*--------------------------------------------------------------------------------
Function    :   Clear_SDA
Purpose     :   Set SDA as Out put, logic LOW, which serves to set the SDA to 0. The pin is also exlicityly set to low.
Parameters  :   None
Return      :   None
Calls       :   Called multiple times per I2C cycle. Should not be called outside of I2C context.
--------------------------------------------------------------------------------*/
void Clear_SDA(void){
  //set the SDA as output, low
  GPIO_setConfig(SDA2, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
}

/*--------------------------------------------------------------------------------
Function    :   I2C_Init
Purpose     :   Initialize I2C block
Parameters  :   None
Return      :   None
Calls       :   Called once per MCU initialization to setup variables for
                data transfer using I2C bitbang.
--------------------------------------------------------------------------------*/
void I2C_Init(void){
 /*
  * To avoid using prohibited names, the variable for the SCL is called SCL2.
  * The variable for SDA is called SDA2.
  * Set the SCL and SDA as GPIO inputs with pull-up resistors and no interrupts on the pins.
  */
 GPIO_setConfig(SCL2, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_NONE);
 GPIO_setConfig(SDA2, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_NONE);
}

/*--------------------------------------------------------------------------------
Function    :   I2C_Start
Purpose     :   Send start signal to begin I2C transmission.
Parameters  :   None
Return      :   None
Calls       :   Called by WriteData and ReadData. Should not be called outside of the I2C context.
--------------------------------------------------------------------------------*/
 void I2C_Start(void){
  Read_SDA(); //set SDA to 1
  I2C_Delay(); //delay
  Clear_SDA(); //Set SDA to 0
  I2C_Delay();
  Clear_SCL(); //Set SCL to 0
 }


 /*--------------------------------------------------------------------------------
Function    :   I2C_Stop
Purpose     :   Send Stop signal
Parameters  :   None
Return      :   None
--------------------------------------------------------------------------------*/
void I2C_Stop(void)
{
  Clear_SDA();      //set SDA to 0
  I2C_Delay();
  Read_SCL();       //set SCL to 1
  I2C_Delay();      //Extra delays added to ensure stop is registered by slave.
  I2C_Delay();
  I2C_Delay();
  Read_SDA();       //set SDA to 1
}

/*--------------------------------------------------------------------------------
Function    :   I2C_Writebit
Purpose     :   Write bit to I2C bus
Parameters  :   a bit need to write
Return      :   None
Calls       :   Called by WriteByte to individually write bits to the bus.
--------------------------------------------------------------------------------*/
void I2C_Writebit(unsigned char bit)
{
  if(bit){
    Read_SDA(); //write a 1
  }
  else{
    Clear_SDA(); //write a 0
  }
  I2C_Delay();
  Read_SCL();
  I2C_Delay();
  Clear_SCL();
}

/*--------------------------------------------------------------------------------
Function    :   I2C_Readbit
Purpose     :   Read bit from I2C bus
Parameters  :   None
Return      :   unsigned char
Calls       :   Called by ReadByte to individually read bits from the bus. Called by WriteByte and WriteData
--------------------------------------------------------------------------------*/
unsigned char I2C_Readbit(void)
{
  unsigned char bit;
  Read_SDA();   //Let the slave drive data
  I2C_Delay();
  Read_SCL();   //cycle clock to high
  bit = Read_SDA(); // collect the data from the bus
  I2C_Delay();
  Clear_SCL(); //cycle the clock to low
  return bit; //return the read data
}

/*--------------------------------------------------------------------------------
Function    :   I2C_WriteByte
Purpose     :   Write a Byte to I2C bus
Parameters  :   unsigned char Data
Return      :   None
Calls       :   Called by WriteData and ReadData
--------------------------------------------------------------------------------*/
void I2C_WriteByte(unsigned char Data)
{
    unsigned char nBit;

  for(nBit = 0; nBit < 8; nBit++)
  {
    I2C_Writebit((Data & 0x80) != 0);
    Data <<= 1;
  }
  I2C_Readbit(); // Allow device to ACK or NACK
}

/*--------------------------------------------------------------------------------
Function    :   I2C_ReadByte
Purpose     :   Read a Byte to I2C bus
Parameters  :   None
Return      :   unsigned char
Calls       :   Called by ReadData
--------------------------------------------------------------------------------*/
unsigned char I2C_ReadByte(void)
{
  unsigned char Buff = 0;
  unsigned char nBit;

  for(nBit = 0; nBit < 8; nBit++)
  {
    Buff = (Buff << 1) | I2C_Readbit();
  }
  return Buff;
}

/*--------------------------------------------------------------------------------
Function    :   I2C_WriteData
Purpose     :   Write n Byte to I2C bus
Parameters  :   Data        - Pointer to Data need to write
                DeviceAddr  - Device Address
                Register    - Register Address
                nLength     - Number of Bytes needed to write
Return      :   None
Calls       :   Called by the program driving the I2C and requesting data.
--------------------------------------------------------------------------------*/
void I2C_WriteData(unsigned char *Data, unsigned char DeviceAddr, unsigned char Register, unsigned char nLength)
{
  unsigned char nIndex;
  I2C_Start();
  I2C_WriteByte(DeviceAddr << 1);  // byDeviceAddr is 7 bit and command is write
  I2C_WriteByte(Register);

  for(nIndex = 0; nIndex < nLength; nIndex++)
  {
    I2C_WriteByte(*(Data + nIndex));
  }
  I2C_Readbit();
  I2C_Stop();
}

/*--------------------------------------------------------------------------------
Function    :   I2C_ReadData
Purpose     :   Read n Byte from I2C bus
Parameters  :   Buff            - Pointer to Buffer store value
                DeviceAddr      - Device Address
                Register        - Register Address
                nLength         - Number of Byte need to read
Return      :   None
Calls       :   Called by the program driving the I2C and requesting data.
--------------------------------------------------------------------------------*/
void I2C_ReadData(unsigned char *Buff, unsigned char DeviceAddr, unsigned char Register, unsigned char nLength)
{
  unsigned char nIndex;
  I2C_Start();

  I2C_WriteByte(DeviceAddr << 1);
  I2C_WriteByte(Register);
  I2C_Delay();                 // Short delay
  Read_SCL();
  I2C_Start();
  I2C_WriteByte((DeviceAddr << 1) | 1);

  for(nIndex = 0; nIndex < nLength; nIndex++)
  {

    *(Buff + nIndex) = I2C_ReadByte();
      if(nIndex < nLength -1)I2C_Writebit(ACK);
      else I2C_Writebit(NACK);
  }

  I2C_Stop();
}

/*--------------------------------------------------------------------------------
Function        :   ReadIR
Purpose         :   Custom function to read temperature from the MLX90614 IR temperature probe.
Pre-requisites  :   I2C_Init must be called before this function is called.
Return          :   double containing the temperature read from the IR sensor in degrees F.
Calls           :   Called by the main program to get the temperature data.
------------    --------------------------------------------------------------------*/
double ReadIR(void){
    //allocate a buffer for the data
    unsigned char * buff = malloc(3 *sizeof(char));

    //Read the data using the I2C bus, feed the data into buff, device address is 0x5A, RAM location of temp
    // is 0x07. Requesting 3 bytes of data - LSB, MSB, PEC
    I2C_ReadData(buff, 0x5A, 0x07, 3);
    uint8_t msb, lsb;
    lsb = *buff; //access the base of the array
    msb = *(buff+sizeof(char)); //access the next element of the array
    int result = (msb << 8)|lsb;
    if(result & 0x8000){ //if the MSB is 1, then the data is bad and the read failed
        return -255; //-255 indicates failure because this is beyond the range of the temp sensor
    }
    // Data sheet indicates to divide the output by 50 to convert to kelvin
    double output = result / 50.0;
    //K to F
    output = (output - 273.15) * 9.0/5.0 + 32;
    free(buff); //deallocate the memory to prevent a segmentation fault
    return output;
}




