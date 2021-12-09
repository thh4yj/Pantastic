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

#define testMode 0
void testProgram(void); //function that holds tester code


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



/*
 *  ======== mainThread ========
 *  Essentially acts as the Program main
 */
void *mainThread(void *arg0){
    //Initialize all the drivers for the hardware of Pantastic
#if testMode
    testProgram();
#else
    initPantastic();

    //Initialize into the first temperature check state
    FSMStateType CurrentState = FirstTempCheck;

    while(1){
        CurrentState = NextStateFunction(CurrentState);
        OutputFunction(CurrentState);
    }
#endif

}

/*--------------------------------------------------------------------------------
Function        :   testProgram
Purpose         :   Test code to be run when in test mode.
Calls           :   Called only when testMode is set to 1
--------------------------------------------------------------------------------*/
void testProgram(void){
    /*
     * Requirements:
     * 1. Test LED matrix
     * 2. Test alarm
     * 3. Test alarm button
     * 4. Test Laser button
     * 5. Test laser diode
     * 6. Test IR sensor
     */
    initPantastic();

    //double temperature = 0;

    while(1){
        flashMatrix(3); // flash 4 times

        int P15 = GPIO_read(AlarmButton);
        int P61 = GPIO_read(LaserButton);
        int P62 = GPIO_read(Alarm);

        temperature = ReadIR();
        //if room temp then light up 3 lights
        if(temperature > 65 && temperature < 78){
            refreshMatrix(210.0);
        }
        else{ //else light up 1 light
            refreshMatrix(105.0);
        }
        sleep(3);

    }

}
