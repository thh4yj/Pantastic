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
#define testMode 1
/*
 * FSM.c
 *
 *  Created on: Oct 13, 2021
 *      Author: Tyler Hendricks -- 5 Guys One Capstone Project
 */
#include "FSM.h"
#include <Drivers.h>
#include "I2CDriver.h"
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Timer.h>
#include <unistd.h>
#include <WiFi.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

extern int networkReadyFlag;

/* Driver configuration */
#include "ti_drivers_config.h"


/*--------------------------------------------------------------------------------
Function        :   NextStateFunction
Purpose         :   Determine the next state for the state machine
Calls           :   Called in every loop in the main
Params          :   CurrentState, the FSMStateType indicating which state the machine is currently in

--------------------------------------------------------------------------------*/

//Helper function to convert double to char array
void reverse(char* str, int len)
{
    int i = 0, j = len - 1, temp;
    while (i < j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

//Helper function to convert double to char array
int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x) {
        str[i++] = (x % 10) + '0';
        x = x / 10;
    }

    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';

    reverse(str, i);
    str[i] = '\0';
    return i;
}

// Converts a floating-point/double number to a string.
void ftoa(float n, char* res, int afterpoint)
{
    // Extract integer part
    int ipart = (int)n;

    // Extract floating part
    float fpart = n - (float)ipart;

    // convert integer part to string
    int i = intToStr(ipart, res, 0);

    // check for display option after point
    if (afterpoint != 0) {
        res[i] = '.'; // add dot

        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter
        // is needed to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);

        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}

FSMStateType NextStateFunction(FSMStateType CurrentState)
{
    FSMStateType NextState = CurrentState; //init NextState so that it is not undefined.

    switch(CurrentState){
        case MCUSleep:
            NextState = FirstTempCheck;
            break;
        case FirstTempCheck:
            temperature = ReadIR(); //update temperature
            if(networkReadyFlag){
                char temperatureCharData[10];
                ftoa(temperature, temperatureCharData, 1); //convert double to char array
                char newTemperatureCharData[10] =  {temperatureCharData[0], temperatureCharData[1], temperatureCharData[2], temperatureCharData[3], '\0'}; //convert char array to string literal
                strcpy(newTemperatureCharData, temperatureCharData); //copy into new char array because string literal not mutable
                SendData(newTemperatureCharData); //send string literal to create http task thread
            }
            if(temperature >= 100){
                NextState = Activation; //if temp is over 100, turn on the device
                //Connect(&connectionFlag); //connect to the WiFi TODO do we need to disconnect or should we remain connected?
            }
            else{
                NextState = MCUSleep; //else go back to sleep
            }
            break;
        case Activation:
            NextState = TempCycle;
            break;
        case TempCycle:
            temperature = ReadIR(); //update temperature
            if(networkReadyFlag){
                char temperatureCharData[10];
                ftoa(temperature, temperatureCharData, 1);
                char newTemperatureCharData[10] =  {temperatureCharData[0], temperatureCharData[1], temperatureCharData[2], temperatureCharData[3], '\0'};
                strcpy(newTemperatureCharData, temperatureCharData);
                SendData(newTemperatureCharData);
            }
            if(temperature < 100){
                NextState = Debounce; //if temp has dropped below 100 then go to debounce to check if it actually dropped
            }
            else if(alarmFlag){
                NextState = SoundAlarm; //if the alarm timer has triggered then sound the alarm
            }
            else{ //return to cycle
                NextState = TempCycle;
            }
            break;
        case SoundAlarm:
            NextState = TempCycle; //even with the alarm on, keep looping
            break;
        case Debounce:
            if(breakDebounceFlag){
                NextState = TempCycle; //heat has been detected so reenter cycle
            }
            else{
                NextState = MCUSleep; //no heat has been present so sleep
                stopAlarmTimer(); //alarm timer not needed when no heat present

                /*
                 * If the long temperature timer has triggered but the heat source is gone
                 * then set disable the flag to prevent the alarm from turning on when the device turns back on.
                 */
                if(alarmFlag){
                    alarmFlag = 0;
                }
            }
            breakDebounceFlag = 0; //reset the flag for the next time debounce is reached.
            break;
    }

    return NextState;
}


/*--------------------------------------------------------------------------------
Function        :   OutputFunction
Purpose         :   Determine which operations to run based on the current state
Calls           :   Called by the main program during every loop
Params          :   CurrentState, the FSMStateType indicating which state the machine is currently in
--------------------------------------------------------------------------------*/
void OutputFunction(FSMStateType CurrentState)
{
    int i;
    switch(CurrentState){
        case MCUSleep:
            disableMatrix(); //turn off the matrix
            flashMatrix(3); //Flash 3 times to indicate state
            #if testMode
                sleep(5); //in test mode sleep for 5 seconds
            #else
                sleep(15*60); //sleep for 15 minutes before checking again
            #endif
            break;
        case FirstTempCheck:
            flashMatrix(1); //Flash the matrix once to indicate state
            temperature = ReadIR(); //force update the IR sensor
            break;
        case Activation:
            //TODO Init Wifi

            flashMatrix(2); // Flash twice to indicate state
            refreshMatrix(temperature); //turn on the LED matrix according to the temperature
            startAlarmTimer(); // start the one hour timer
            break;
        case TempCycle:
            temperature = ReadIR();
            refreshMatrix(temperature); //turn on the LED matrix according to the temperature
            //TODO Update WiFi UI based on value

            //if sleep drops into low power mode and interferes with wifi, etc, then we can use Timer2
            #if testMode
                sleep(3); //sleep 3 seconds in test mode
            #else
                sleep(30); //take the temperature every 30 seconds
            #endif
            break;
        case SoundAlarm:
            alarmOn();
            break;
        case Debounce:
            //trying this instead of other timer module
            flashMatrix(4);
            for(i = 0; i < 10; i++){
                temperature = ReadIR();
                if(temperature >= 100){
                    breakDebounceFlag = 1;
                    return;
                }
                #if testMode
                    sleep(1);
                #else
                    sleep(30); //sleep 30 seconds before checking again, state will run for 5 minutes
                #endif

            }
            break;
    }

}
