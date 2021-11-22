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
 * FSM.h
 *
 *  Created on: Oct 13, 2021
 *      Author: Tyler Hendricks-- 5 Guys One Capstone Project
 */

#ifndef FSM_H_
#define FSM_H_

#include <stdint.h>

extern double temperature;


typedef enum {
    MCUSleep,
    FirstTempCheck,
    Activation,
    TempCycle,
    SoundAlarm,
    Debounce
}FSMStateType;

FSMStateType NextStateFunction(FSMStateType CurrentState);

void OutputFunction(FSMStateType CurrentState);

extern int debounceFlag; // When set to 1, the FSM can enter MCU sleep
extern int breakDebounceFlag; //When set to 1, the FSM will break out of debounce
extern int alarmFlag; // When set to 1, the alarm will ring during the temp cycle
extern int alarmCycle;




#endif /* FSM_H_ */
