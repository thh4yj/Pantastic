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
 * Drivers.c
 *
 *  Created on: Nov 21, 2021
 *      Author: Tyler Hendricks (thh4yj)
 *      Team: 5 Guys One Capstone Project/ Pantastic/ The Pantastic 5
 */

#include "Drivers.h"
#include "I2CDriver.h"
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Timer.h>
#include <FSM.h>

/* Driver configuration */
#include "ti_drivers_config.h"

int debounceFlag; // When set to 1, the FSM can enter MCU sleep
int breakDebounceFlag; //When set to 1, the FSM will break out of debounce
int alarmFlag; // When set to 1, the alarm will ring during the temp cycle
int alarmCycle;
double temperature;

/*--------------------------------------------------------------------------------
Function        :   initPantastic
Purpose         :   Initialize the drivers for the hardware needed for Pantastic operation.
Calls           :   Called by the main program once to initialize hardware.
Pre-req         :   Source code must have hardware pins defined in ti_drivers_config.h/c
--------------------------------------------------------------------------------*/
void initPantastic(void){
    GPIO_init();
    Timer_init();
    I2C_Init(); //Initialize the hardware to use the custom I2C bus
    initLEDMatrix(); //Initialize the LED heat visualizer matrix
    initAlarmButton(); //Initialize the button that silences the alarm
    initLaserButton(); //Initialize the button that turns on the laser diode
    initAlarm(); //Initialize the alarm module for sounding the alarm
    initLaser(); //Initialize the laser module for aiming the IR sensor
    initDebounceTimer(); //Initialize the timer module for the debounce timer
    initAlarmTimer(); //Initialize the timer module for the alarm timer

    debounceFlag = 0; // When set to 1, the FSM can enter MCU sleep
    breakDebounceFlag = 0; //When set to 1, the FSM will break out of debounce
    alarmFlag = 0; // When set to 1, the alarm will ring during the temp cycle
    alarmCycle = 0;
    temperature = 0;
}

// ************** Driver Initializations *************************

/*--------------------------------------------------------------------------------
Function        :   initLEDMatrix
Purpose         :   Initialize the GPIO pins for the LED heat matrix.
Pins            :   Reference Designators on board ended up with confusing names. Format:
                    Name -> Pin -> Name on Board
                    LED1 -> P03 -> LED12
                    LED2 -> P05 -> LED7
                    LED3 -> P06 -> LED8
                    LED4 -> P07 -> LED9
                    LED5 -> P08 -> LED10
                    LED6 -> P18 -> LED11

Calls           :   Called by initPantastic function.
Configuration   :   All pins are configured as GPIO outputs, high drive strength, and initially set to low.
--------------------------------------------------------------------------------*/
void initLEDMatrix(void){
    GPIO_setConfig(LED1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(LED2, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(LED3, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(LED4, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(LED5, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(LED6, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW);
}

/*--------------------------------------------------------------------------------
Function        :   initAlarmButton
Purpose         :   Initialize the GPIO pin that sense the button press to turn off the alarm.
Pin             :   P21
Calls           :   Called by initPantastic function.
Configuration   :   Utilizes header pin P21. Sets as an input, no pull resistors, with a falling edge interrupt.
                    No pull resistors are used because external pull-up circuitry is used for this input.
                    The external circuitry uses pull up resistors, filters out noise and electrostatic discharge to prevent
                    unintentional triggers. The default value is 1, the triggered value is 0.
--------------------------------------------------------------------------------*/
void initAlarmButton(void){
    GPIO_setConfig(AlarmButton, GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_FALLING);
}

/*--------------------------------------------------------------------------------
Function        :   initLaserButton
Purpose         :   Initialize the GPIO pin that sense the button press to turn on the laser diode.
Pin             :   P45
Calls           :   Called by initPantastic function.
Configuration   :   Utilizes header pin P45. Sets as an input, no pull resistors, with a falling edge interrupt.
                    No pull resistors are used because external pull-up circuitry is used for this input.
                    The external circuitry uses pull up resistors, filters out noise and electrostatic discharge to prevent
                    unintentional triggers. The default value is 1, the triggered value is 0.
--------------------------------------------------------------------------------*/
void initLaserButton(void){
    GPIO_setConfig(LaserButton, GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_FALLING);
}

/*--------------------------------------------------------------------------------
Function        :   initAlarm
Purpose         :   Initialize the GPIO pin that controls the alarm module.
Pin             :   P62
Calls           :   Called by initPantastic function.
Configuration   :   Utilizes header pin P62. Sets as an output with high drive strength and
                    initially is set to low.
--------------------------------------------------------------------------------*/
void initAlarm(void){
    GPIO_setConfig(Alarm, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW);
}

/*--------------------------------------------------------------------------------
Function        :   initLaser
Purpose         :   Initialize the GPIO pin that controls the laser diode.
Pin             :   P50
Calls           :   Called by initPantastic function.
Configuration   :   Utilizes header pin P50. Sets as an output with high drive strength and
                    initially is set to low.
--------------------------------------------------------------------------------*/
void initLaser(void){
    GPIO_setConfig(LaserDiode, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW);
}


// ******************* Driver Methods *********************************


/*--------------------------------------------------------------------------------
Function        :   laserOn
Purpose         :   Turn on the laser diode used for aiming.
Pin             :   P50
Calls           :   Called by the main program as many times as needed.
Pre-req         :   Must be initialized first.
Configuration   :   P50 is output, drive strength high, initially low
--------------------------------------------------------------------------------*/
void laserOn(void){
    GPIO_write(LaserDiode, 1); //Write a 1 to the GPIO pin to turn it on
}

/*--------------------------------------------------------------------------------
Function        :   laserOff
Purpose         :   Turn off the laser diode used for aiming.
Pin             :   P50
Calls           :   Called by the main program as many times as needed.
Pre-req         :   Must be initialized first.
Configuration   :   P50 is output, drive strength high, initially low
--------------------------------------------------------------------------------*/
void laserOff(void){
    GPIO_write(LaserDiode, 0); //Write a 0 to the GPIO pin to turn it off
}

/*--------------------------------------------------------------------------------
Function        :   alarmOn
Purpose         :   Turn on the siren used to alert the user
Pin             :   P62
Calls           :   Called by the main program as many times as needed.
Pre-req         :   Must be initialized first.
Configuration   :   P62 is output, drive strength high, initially low
--------------------------------------------------------------------------------*/
void alarmOn(void){
    /*
     * TODO: Need to determine how the alarm module will function:
     * Option 1: Square wave- this method will required a toggle and a delay to be called somehwere
     * Option 2: Active buzzer - this option will require simply setting the pin to high
     */
    GPIO_toggle(Alarm); //toggle the state of the alarm
}

/*--------------------------------------------------------------------------------
Function        :   alarmOff
Purpose         :   Turn off the siren used to alert the user
Pin             :   P62
Calls           :   Called by the main program as many times as needed.
Pre-req         :   Must be initialized first.
Configuration   :   P62 is output, drive strength high, initially low
--------------------------------------------------------------------------------*/
void alarmOff(void){
    /*
     * TODO: Update function based on methodology decided on in alarmOn
     */
    GPIO_write(Alarm, 0); //set the alarm to off

}

/*--------------------------------------------------------------------------------
Function        :   flashMatrix
Purpose         :   Flash the LED matrix a given number of times (all 6 will flash). Returns LEDs to prior state.
Param           :   flashNum, an integer of the number of times to flash, must be greater than 0.
Pins            :   P03, P05, P06, P07, P08, P18
Calls           :   Called by the main program as many times as needed.
Pre-req         :   Must be initialized first.
Configuration   :   Pins set as output, drive strength high, low
--------------------------------------------------------------------------------*/
void flashMatrix(int flashNum){
    //read and hold the LEDs state
    int delayTime = 500000;
    int s1,s2,s3,s4,s5,s6;
    s1 = GPIO_read(LED1);
    s2 = GPIO_read(LED2);
    s3 = GPIO_read(LED3);
    s4 = GPIO_read(LED4);
    s5 = GPIO_read(LED5);
    s6 = GPIO_read(LED6);

    int i;
    for(i = 0; i < flashNum; i++){
        //Blink on all LEDs
        GPIO_write(LED1, 1);
        GPIO_write(LED2, 1);
        GPIO_write(LED3, 1);
        GPIO_write(LED4, 1);
        GPIO_write(LED5, 1);
        GPIO_write(LED6, 1);

        //Delay for 500,000 microseconds = 500 ms = 0.5 seconds
        delayXus(delayTime);

        //Blink off all LEDs
        GPIO_write(LED1, 0);
        GPIO_write(LED2, 0);
        GPIO_write(LED3, 0);
        GPIO_write(LED4, 0);
        GPIO_write(LED5, 0);
        GPIO_write(LED6, 0);

        //delay
        delayXus(delayTime);
    }

    //Restore the LEDs to the original state
    GPIO_write(LED1, s1);
    GPIO_write(LED2, s2);
    GPIO_write(LED3, s3);
    GPIO_write(LED4, s4);
    GPIO_write(LED5, s5);
    GPIO_write(LED6, s6);
}

/*--------------------------------------------------------------------------------
Function        :   disableMatrix
Purpose         :   Turn off the LED matrix
Pin             :   P03, P05, P06, P07, P08, P18
Calls           :   Called by the main program as many times as needed.
Pre-req         :   Must be initialized first.
Configuration   :   Pins set as output, drive strength high, low
--------------------------------------------------------------------------------*/
void disableMatrix(void){
    GPIO_write(LED1, 0);
    GPIO_write(LED2, 0);
    GPIO_write(LED3, 0);
    GPIO_write(LED4, 0);
    GPIO_write(LED5, 0);
    GPIO_write(LED6, 0);
}

/*--------------------------------------------------------------------------------
Function        :   refreshMatrix
Purpose         :   Update how many LEDs should be turned on based on the heat.
Param           :   temp, a double indicating the IR temperature reading in degrees F
Pin             :   P03, P05, P06, P07, P08, P18
Calls           :   Called by the main program as many times as needed.
Pre-req         :   Must be initialized first.
Configuration   :   Pins set as output, drive strength high, low
--------------------------------------------------------------------------------*/
void refreshMatrix(double temp){
    //TODO Idea for increased functionality could be to adjust drive strength to adjust brightness
    //This allows for instead of 6 temperature zones for there to be 18

    //If temp is above base threshold, turn on the first LED, else turn off all LEDs
    if(temp < 100){
        disableMatrix();
        return;
    }
    else{
        GPIO_write(LED1, 1);
    }

    // If temp is above 150, LED1 is on and LED2 will turn on, else LED2 will be turned off
    if(temp > 150){
        GPIO_write(LED2, 1);
    }
    else{
        GPIO_write(LED2, 0);
    }

    //If temp is above 200, LED1, LED2 are on and LED3 will turn on, else LED3 will be turned off
    if(temp > 200){
        GPIO_write(LED3, 1);
    }
    else{
        GPIO_write(LED3, 0);
    }

    if(temp > 250){
        GPIO_write(LED4, 1);
    }
    else{
        GPIO_write(LED4, 0);
    }

    if(temp > 300){
        GPIO_write(LED5, 1);
    }
    else{
        GPIO_write(LED5, 0);
    }
    if(temp > 350){
        GPIO_write(LED6, 1);
    }
    else{
        GPIO_write(LED6, 0);
    }
}

/*--------------------------------------------------------------------------------
Function        :   initDebounceTimer
Purpose         :   Initialize the debounce timer and its parameters
Module          :   Timer0
Calls           :   Called by initPantastic
Pre-req         :   Timer Module must be initialized first. This is done by initPantastic
Configuration   :   Set to a 5 minute period, period set to microseconds, set to ONESHOT which will allow the main thread
                    to continue running. When an interrupt is generated, the timer will stop and not begin until it is
                    explicitly started.
--------------------------------------------------------------------------------*/
void initDebounceTimer(void){
    Timer_Params_init(&paramsDeb);
    paramsDeb.period = 5 * 60 * 1000000; // 5 * 60 * 1,000,000 = 5 minutes
    paramsDeb.periodUnits = Timer_PERIOD_US;
    paramsDeb.timerMode = Timer_ONESHOT_CALLBACK;
    paramsDeb.timerCallback = debounceCallback;

    debTimer = Timer_open(DebounceTimer, &paramsDeb);

}

/*--------------------------------------------------------------------------------
Function        :   debounceCallback
Purpose         :   Function called when the interrupt for the debounce Timer is triggered
Calls           :   Called by timer interrupt
--------------------------------------------------------------------------------*/
void debounceCallback(Timer_Handle myHandle, int_fast16_t status){
    debounceFlag = 1;
    Timer_stop(debTimer); //may not need this in ONESHOT mode
}

/*--------------------------------------------------------------------------------
Function        :   initAlarmTimer
Purpose         :   Initialize the alarm timer and its parameters
Module          :   Timer1
Calls           :   Called by initPantastic
Pre-req         :   Timer Module must be initialized first. This is done by initPantastic
Configuration   :   Set to a 30 minute period, period set to microseconds, set to ONESHOT which will allow the main thread
                    to continue running. When an interrupt is generated, the timer will stop and not begin until it is
                    explicitly started.
--------------------------------------------------------------------------------*/
void initAlarmTimer(void){
    Timer_Params_init(&paramsAlarm);
    paramsAlarm.period = 1800 * 1000000; // 60 * 30 * 1,000,000 = execute every 30 minutes
    paramsAlarm.periodUnits = Timer_PERIOD_US;
    paramsAlarm.timerMode = Timer_ONESHOT_CALLBACK;
    paramsAlarm.timerCallback = alarmCallback;

    longHeatTimer = Timer_open(AlarmTimer, &paramsAlarm);
}

/*--------------------------------------------------------------------------------
Function        :   alarmCallback
Purpose         :   Function called when the interrupt for the heat alarm Timer is triggered
Calls           :   Called by timer interrupt
--------------------------------------------------------------------------------*/
void alarmCallback(Timer_Handle myHandle, int_fast16_t status){
    Timer_stop(longHeatTimer); //Stop the timer, may not be needed in ONESHOT mode
    alarmCycle++; //increment the number of cycles, 2 needed for 60 minutes
    if(alarmCycle > 1){ //if one cycle is present then 60 minutes has been reached
        alarmFlag = 1; //set the flag to trigger the alarm
        alarmCycle = 0; //reset the counter for the next timer
    }
    else{
        Timer_start(longHeatTimer); //if more than one cycle has not run, then restart the timer for the second cycle
    }
}

/*--------------------------------------------------------------------------------
Function        :  StartDebounceTimer
Purpose         :  Start the timer for debouncing detection
Calls           :  Called by anything
Pre-req         :  Timer must be initialized
--------------------------------------------------------------------------------*/
void startDebounceTimer(void){
    Timer_start(debTimer);
}

/*--------------------------------------------------------------------------------
Function        :  stopDebounceTimer
Purpose         :  Stop the timer for debouncing
Calls           :  Can be called by anything
Pre-req         :  Timer must be initialized
--------------------------------------------------------------------------------*/
void stopDebounceTimer(void){
    Timer_stop(debTimer);
}

/*--------------------------------------------------------------------------------
Function        :  startAlarmTimer
Purpose         :  Starts the one hour heat alarm timer
Calls           :  Can be called by anything
Pre-req         :  Timer must be initialized
--------------------------------------------------------------------------------*/
void startAlarmTimer(void){
    Timer_start(longHeatTimer);
}

/*--------------------------------------------------------------------------------
Function        :  stopAlarmTimer
Purpose         :  Stop the heat alarm that sounds when there is high heat for over an hour
Calls           :  Can be called by anything
Pre-req         :  Timer must be initialized
--------------------------------------------------------------------------------*/
void stopAlarmTimer(void){
    Timer_stop(longHeatTimer);
}

/*--------------------------------------------------------------------------------
Function        :   delayXus
Purpose         :   Do nothing (delay) for x microseconds.
Param           :   X, an integer specifiying the number of microseconds to delay
Calls           :   Called by the main program as many times as needed.
--------------------------------------------------------------------------------*/
void delayXus(int x){
    int i;
    int k;
    for(k = 0; k < x; k++){
        for(i = 0; i < 80; i++){
            __asm("nop \n"); //generate assembly NOP.
        }
    }
}
