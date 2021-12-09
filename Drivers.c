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


int breakDebounceFlag; //When set to 1, the FSM will break out of debounce
int alarmFlag; // When set to 1, the alarm will ring during the temp cycle
int alarmCycle; //The number of alarm cycles that have been reached. This allows larger times to be reached by the timer
// For example timer is set to 30 seconds. 4 alarm cycles indicates that 2 minutes have been reached.
double temperature; // Temperature value, if needed. Different from temp value in FSM

/*
 * Indicates value of the respective button.
 * Pullup resistors are used so:
 * Not Pushed   = 1
 * Pushed       = 0
 */
volatile int laserButtonFlag;
volatile int alarmButtonFlag;

/*
 * Test mode is essentially demo mode.
 * It uses shorter timer values to make testing much easier.
 *
 * Button interrupt mode allows interrupts on the buttons to be turned back on.
 * This is disabled to allow for debouncing the buttons.
 */
#define testMode 1
#define btnInterruptMode 0


/*--------------------------------------------------------------------------------
Function        :   initPantastic
Purpose         :   Initialize the drivers for the hardware needed for Pantastic operation.
Calls           :   Called by the main program once to initialize hardware.
Pre-req         :   Source code must have hardware pins defined in ti_drivers_config.h/c
--------------------------------------------------------------------------------*/
void initPantastic(void){
    GPIO_init(); //Must be called to use any GPIO functions
    Timer_init(); //Must be called to use any timer functions
    I2C_Init(); //Initialize the hardware to use the custom I2C bus
    initLEDMatrix(); //Initialize the LED heat visualizer matrix
    initAlarmButton(); //Initialize the button that silences the alarm
    initLaserButton(); //Initialize the button that turns on the laser diode
    initAlarm(); //Initialize the alarm module for sounding the alarm
    initLaser(); //Initialize the laser module for aiming the IR sensor
    initAlarmTimer(); //Initialize the timer module for the alarm timer
    initButtonDebounceTimer(); //Periodic timer to check the button push values. Automatically debounces button pushes.

    breakDebounceFlag = 0; //When set to 1, the FSM will break out of debounce
    alarmFlag = 0; // When set to 1, the alarm will ring during the temp cycle
    alarmCycle = 0; //number of alarm timer cycles run
    temperature = 0;

    laserButtonFlag = 1; //set default to not pushed
    alarmButtonFlag = 1;

    /*
     * Current default is not to use button interrupt mode to enable debouncing for switches by using
     * a periodic timer to check the push values.
     */
    #if !btnInterruptMode
        Timer_start(buttonDebounce);
    #endif
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
Pin             :   P21 bridged to P58*. Note the alarm was incorrectly connected to P21* in the circuit
Calls           :   Called by initPantastic function.
Configuration   :   Utilizes header pin P21. Sets as an input, no pull resistors, with a falling edge interrupt.
                    No pull resistors are used because external pull-up circuitry is used for this input.
                    The external circuitry uses pull up resistors, filters out noise and electrostatic discharge to prevent
                    unintentional triggers. The default value is 1, the triggered value is 0.

                    When btnInterruptMode is disabled, the button is an pullup input with no interrupt.
--------------------------------------------------------------------------------*/
void initAlarmButton(void){
#if btnInterruptMode
    //Set pullup GPIO pin with falling edge interrupt
    GPIO_setConfig(AlarmButton, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
    //Set trigger function to HandleAlarmPush
    GPIO_setCallback(AlarmButton, HandleAlarmPush);
    GPIO_enableInt(AlarmButton);
#else
    //Set pullup GPIO pin with no interrupt
    GPIO_setConfig(AlarmButton, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_NONE);
#endif
}


/*--------------------------------------------------------------------------------
Function        :   HandleLaserButton
Purpose         :   Function that is called by the GPIO interrupt generated when the button is pushed.
                    Function that is called by a periodic timer interrupt when not in interrupt mode.
Calls           :   Called by an interrupt handler in btnInterruptMode
                    Called by a periodic timer when not in interrupt mode
--------------------------------------------------------------------------------*/
volatile int laserState = 0; //global variable to save laser state
void HandleLaserButton(uint_least8_t index){
    #if btnInterruptMode //run if in interrupt mode
        flashMatrix(1);
        if(laserState){ // Checking toggle state
            laserOff(); // Turn off the laser and toggle the state
            laserState = 0;
        }
        else{
            laserOn(); // Turn on the laser and toggle the state
            laserState = 1;
        }
    #else //run if not in interrupt mode

        //only run code if the laser button has been pushed
        if(laserButtonFlag == 0){
            flashMatrix(1); //flash the matrix to indicate a button was pushed
            if(laserState){ // Checking toggle state
                laserOff(); // Turn off the laser and toggle the state
                laserState = 0;
            }
            else{
                laserOn(); // Turn on the laser and toggle the state
                laserState = 1;
            }
        }
    #endif
}

/*--------------------------------------------------------------------------------
Function        :   initLaserButton
Purpose         :   Initialize the GPIO pin that sense the button press to turn on the laser diode.
Pin             :   P45 bridged to P60
Calls           :   Called by initPantastic function.
Configuration   :   Utilizes header pin P45. Sets as an input, no pull resistors, with a falling edge interrupt.
                    No pull resistors are used because external pull-up circuitry is used for this input.
                    The external circuitry uses pull up resistors, filters out noise and electrostatic discharge to prevent
                    unintentional triggers. The default value is 1, the triggered value is 0.

                    If btnInterruptMode is disabled then the button is an input with a pullup resistor.
--------------------------------------------------------------------------------*/
void initLaserButton(void){
#if btnInterruptMode
    //set as input GPIO pin with pullup and a falling edge interrupt
    GPIO_setConfig(LaserButton, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
    //Set trigger function to be HandleLaser Button
    GPIO_setCallback(LaserButton, HandleLaserButton);
    GPIO_enableInt(LaserButton);
#else
    //set as input pin with pullup and no interrupt
    GPIO_setConfig(LaserButton, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_NONE);
#endif
}


/*--------------------------------------------------------------------------------
Function        :   HandleAlarmPush
Purpose         :   Function that is called by the GPIO interrupt generated when the button is pushed.
                    Function that is called by the timer handler when the time is reached when not in interrupt mode
Calls           :   Called by an interrupt handler in interrupt mode
                    Called by a periodic timer when not in interrupt mode
--------------------------------------------------------------------------------*/
volatile int alarmButtonPush = 0;
void HandleAlarmPush(uint_least8_t index){
    #if testMode //if in demo mode
        #if btnInterruptMode //if in demo and interrupt mode run this
            flashMatrix(1); //flash matrix to indicate button was pushed
            alarmOff(); //turn off the alarm
            alarmFlag = 0; //set the alarm flag to 0 so the alarm will not turn on again unless the timer triggers it
            /*
             * Restart the alarm timer. Will reset the timer if pushed when the alarm
             * is not ringing.
             */
            startAlarmTimer();
        #else //if in demo mode and not in interrupt mode run this

            //only run code if the alarm button has been pushed
            if(alarmButtonFlag == 0){
                flashMatrix(1); //flash the matrix to indicate the button has been pushed
                alarmOff(); // Turn off the alarm.
                alarmFlag = 0; //set the alarm flag to 0 so the alarm will not turn on again unless the timer triggers it
                /*
                 * Restart the alarm timer. Will reset the timer if pushed when the alarm
                 * is not ringing.
                 */
                startAlarmTimer();

            }
        #endif
    #else //if not in demo mode

        //only run code if alarm button has been pushed
        #if btnInterruptMode //if not in demo mode but in interrupt mode run this
            flashMatrix(1); //flash matrix to indicate button was pushed
            alarmOff(); //turn off the alarm
            alarmFlag = 0; //set the alarm flag to 0 so the alarm will not turn on again unless the timer triggers it
            /*
             * Restart the alarm timer. Will reset the timer if pushed when the alarm
             * is not ringing.
             */
            startAlarmTimer();
        #else //if not demo mode and not in interrupt mode run this
            if(alarmButtonFlag == 0){
                flashMatrix(1); //flash matrix to indicate button was pushed
                alarmOff(); //turn off the alarm
                alarmFlag = 0; //set the alarm flag to 0 so the alarm will not turn on again unless the timer triggers it
                /*
                 * Restart the alarm timer. Will reset the timer if pushed when the alarm
                 * is not ringing.
                 */
                startAlarmTimer();
            }
        #endif

    #endif
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
    GPIO_write(LaserDiode, 0);
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
    GPIO_write(Alarm, 1); //set the alarm to on
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
    int delayTime = 15000;
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

        //Delay
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
Function        :   initButtonDebounceTimer
Purpose         :   Initialize the button debounce timer and its parameters
Module          :   Timer2
Calls           :   Called by initPantastic
Pre-req         :   Timer Module must be initialized first. This is done by initPantastic
Configuration   :   Set to a 10ms period, period set to microseconds, set to continuous which will allow the main thread
                    to continue running. When an interrupt is generated, the timer will stop, run the function, and restart
--------------------------------------------------------------------------------*/
void initButtonDebounceTimer(void){
    Timer_Params_init(&paramsButton);
    paramsButton.period = 100000;
    paramsButton.periodUnits = Timer_PERIOD_US;
    paramsButton.timerMode = Timer_CONTINUOUS_CALLBACK;
    paramsButton.timerCallback = buttonDebounceCallback;

    buttonDebounce = Timer_open(ButtonDebounceConifg, &paramsButton);
}

/*--------------------------------------------------------------------------------
Function        :   buttonDebounceCallback
Purpose         :   Function called when the interrupt for the button debounce Timer is triggered.
                    Updates the flag to indicate if a button has been pushed and to call the handlers for
                    the buttons.
Calls           :   Called by timer interrupt
Magic Number    :   The parameter for the button callbacks is arbitrary. The number is based on the requirements for
                    the function to be a callback for an interrupt.
--------------------------------------------------------------------------------*/
void buttonDebounceCallback(Timer_Handle myHandle, int_fast16_t status){
    laserButtonFlag = GPIO_read(LaserButton); //read the laser button state and store the state
    alarmButtonFlag = GPIO_read(AlarmButton); //read the alarm button state
    HandleLaserButton(0); //based on how the functions work the param is arbitrary
    HandleAlarmPush(0);
}

/*--------------------------------------------------------------------------------
Function        :   initAlarmTimer
Purpose         :   Initialize the alarm timer and its parameters
Module          :   Timer1
Calls           :   Called by initPantastic
Pre-req         :   Timer Module must be initialized first. This is done by initPantastic
Configuration   :   Set to a 30 second period, period set to microseconds, set to ONESHOT which will allow the main thread
                    to continue running. When an interrupt is generated, the timer will stop and not begin until it is
                    explicitly started.
--------------------------------------------------------------------------------*/
void initAlarmTimer(void){
    Timer_Params_init(&paramsAlarm);
    paramsAlarm.period = 30 * 1000000; //30 seconds
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
    #if testMode
        if(alarmCycle > 4){ //if 4 cycles are present then 60 minutes has been reached
            alarmFlag = 1; //set the flag to trigger the alarm
            alarmCycle = 0; //reset the counter for the next timer
        }
        else{
            Timer_start(longHeatTimer); //if more than one cycle has not run, then restart the timer for the second cycle
        }
    #else
        if(alarmCycle > 120){ //if 120 cycles are present then 60 minutes has been reached
            alarmFlag = 1; //set the flag to trigger the alarm
            alarmCycle = 0; //reset the counter for the next timer
        }
        else{
            Timer_start(longHeatTimer); //if more than one cycle has not run, then restart the timer for the second cycle
        }
    #endif
}

/*--------------------------------------------------------------------------------
Function        :  startAlarmTimer
Purpose         :  Starts the one hour heat alarm timer
Calls           :  Can be called by anything
Pre-req         :  Timer must be initialized
--------------------------------------------------------------------------------*/
void startAlarmTimer(void){
    int stat = Timer_start(longHeatTimer);

    /*
     * If an error is returned then the timer is already running. This means the user
     * pushed the button when the alarm was not sounding. This statement allows the user to restart the timer
     * by pushing the alarm button.
     */
    if(stat == Timer_STATUS_ERROR){
        Timer_stop(longHeatTimer); //stop the timer to restart it.
        alarmCycle = 0; //reset the number of cycles.
        Timer_start(longHeatTimer); //start the timer again
    }
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
