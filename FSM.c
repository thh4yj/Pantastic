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

/* Driver configuration */
#include "ti_drivers_config.h"

/*--------------------------------------------------------------------------------
Function        :   NextStateFunction
Purpose         :   Determine the next state for the state machine
Calls           :   Called in every loop in the main
Params          :   CurrentState, the FSMStateType indicating which state the machine is currently in
--------------------------------------------------------------------------------*/
FSMStateType NextStateFunction(FSMStateType CurrentState)
{
    FSMStateType NextState = CurrentState; //init NextState so that it is not undefined.

    switch(CurrentState){
        case MCUSleep:
            NextState = FirstTempCheck;
            break;
        case FirstTempCheck:
            if(temperature >= 100){
                NextState = Activation;
            }
            else{
                NextState = MCUSleep;
            }
            break;
        case Activation:
            NextState = TempCycle;
            Timer_start(longHeatTimer);
            break;
        case TempCycle:
            if(temperature < 100){
                NextState = Debounce;
                Timer_start(debTimer)
            }
            else if(alarmFlag){
                NextState = SoundAlarm;
            }
            else{
                NextState = TempCycle;
            }
            break;
        case SoundAlarm:
            NextState = TempCycle;
            break;
        case Debounce:
            if(breakDebounceFlag){
                NextState = TempCycle;
                Timer_stop(debTimer);
            }
            else{
                NextState = MCUSleep;
                Timer_stop(debTimer);
                Timer_stop(longHeatTimer);
            }
            breakDebounceFlag = 0;
//            if(temperature >= 100){
//                NextState = TempCycle;
//            }
//            else if(debounceFlag){
//                NextState = MCUSleep;
//            }
//            else{
//                NextState = Debounce;
//            }
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
            sleep(15*60); //sleep for 15 minutes before checking again
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
            sleep(30); //take the temperature every 30 seconds
            break;
        case SoundAlarm:
            alarmOn();
            break;
        case Debounce:
            //trying this instead of other timer module
            for(i = 0; i < 10; i++){
                temperature = ReadIR();
                if(temperature >= 100){
                    breakDebounceFlag = 1;
                    return;
                }
                sleep(30); //sleep 30 seconds before checking again, state will run for 5 minutes
            }
            break;
    }

}
