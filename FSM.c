/*
 * FSM.c
 *
 *  Created on: Oct 13, 2021
 *      Author: Tyler Hendricks -- 5 Guys One Capstone Project
 */
#include "FSM.h"

//FSM Chart
//

FSMStateType NextStateFunction(FSMStateType CurrentState, uint16_t temperature)
{
    FSMStateType NextState = CurrentState; //init NextState so that it is not undefined.

    if(CurrentState == Init){

    }
    else if(CurrentState == MCUSleep){

    }
    else if(CurrentState == MCUWake){

    }
    else if(CurrentState == FirstTempCheck){

    }
    else if(CurrentState == Activation){

    }
    else if(CurrentState == TempCycle){

    }
    else if(CurrentState == SoundAlarm){

    }
    else if(CurrentState == DisableAlarm){

    }
    else{ //important to have a default case but this case should never be reached. Maybe trigger a reset if this is hit?

    }

}





void OutPutFunction(FSMStateType CurrentState)
{


}




