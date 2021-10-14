/*
 * FSM.h
 *
 *  Created on: Oct 13, 2021
 *      Author: Tyler Hendricks-- 5 Guys One Capstone Project
 */

#ifndef FSM_H_
#define FSM_H_

#include <stdint.h>

typedef enum {
    Init,
    MCUSleep,
    MCUWake,
    FirstTempCheck,
    Activation,
    TempCycle,
    SoundAlarm,
    DisableAlarm
}FSMStateType;

FSMStateType NextStateFunction(FSMStateType CurrentState, uint16_t temperature);

void OutputFunction(FSMStateType CurrentState);



#endif /* FSM_H_ */
