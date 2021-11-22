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
 *  ======== gpiointerrupt.c ========
 */
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/display/Display.h>
#include <I2CDriver.h>
#include <FSM.h>
#include <ti/drivers/Timer.h>
#include <Drivers.h>


/* Driver configuration */
#include "ti_drivers_config.h"

int laserState = 0;

/*--------------------------------------------------------------------------------
Function        :   HandleAlarmPush
Purpose         :   Function that is called by the GPIO interrupt generated when the button is pushed.
Calls           :   Called by an interrupt handler.
--------------------------------------------------------------------------------*/
void HandleAlarmPush(void){
    alarmOff(); // Turn off the alarm.
    startAlarmTimer(); //restart the alarm timer
}

/*--------------------------------------------------------------------------------
Function        :   HandleLaserButton
Purpose         :   Function that is called by the GPIO interrupt generated when the button is pushed.
Calls           :   Called by an interrupt handler
--------------------------------------------------------------------------------*/
void HandleLaserButton(void){
    if(laserState){ // Checking toggle state
        laserOff(); // Turn off the laser and toggle the state
        laserState = 0;
    }
    else{
        laserOn(); // Turn on the laser and toggle the state
        laserState = 1;
    }
}

/*
 *  ======== mainThread ========
 *  Essentially acts as the Program main
 */
void *mainThread(void *arg0){
    //Initialize all the drivers for the hardware of Pantastic
    void initPantastic();

    //Initialize into the first temperature check state
    FSMStateType CurrentState = FirstTempCheck;

    while(1){
        CurrentState = NextStateFunction(CurrentState);
        OutputFunction(CurrentState);
    }
}


