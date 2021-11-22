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
 * Drivers.h
 *
 *  Created on: Nov 21, 2021
 *      Author: Tyler Hendricks (thh4yj)
 *
 *      Header file for the drivers of the Pantastic.
 *
 *      For simplicity, all modules can be initialized at once by calling initPantastic(). This is the recommended
 *      usage over individually calling the initialization functions.
 */

#ifndef DRIVERS_H_
#define DRIVERS_H_

#include <ti/drivers/Timer.h>


//--- Main driver initialization function ---
void initPantastic(void);

//-------

//--- Module level initialization functions ---
void initLEDMatrix(void);
void initAlarmButton(void);
void initLaserButton(void);
void initAlarm(void);
void initLaser(void);

//-------

//--- Module level usage/activation calls ---
void laserOn(void);
void laserOff(void);

void alarmOn(void);
void alarmOff(void);

void flashMatrix(int flashNum);
void disableMatrix(void);
void refreshMatrix(double temp);

void initDebounceTimer(void);
void initAlarmTimer(void);
void startDebounceTimer(void);
void stopDebounceTimer(void);
void startAlarmTimer(void);
void stopAlarmTimer(void);

//Header values for the timer
Timer_Handle debTimer;
Timer_Params paramsDeb;

Timer_Handle longHeatTimer;
Timer_Params paramsAlarm;

void debounceCallback(Timer_Handle myHandle, int_fast16_t status);
void alarmCallback(Timer_Handle myHandle, int_fast16_t status);

// --- Helper functions ---
void delayXus(int x);




#endif /* DRIVERS_H_ */
