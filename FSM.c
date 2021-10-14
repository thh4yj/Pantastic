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
        NextState = MCUWake;
//        if(temperature >= 100){
//            NextState = MCUWake;
//        }
//        else{
//            NextState = MCUSleep;
//        }

    }
    else if(CurrentState == MCUWake){
        NextState = FirstTempCheck;
    }
    else if(CurrentState == FirstTempCheck){
        if(temperature >= 100){
            NextState = Activation;
        }
        else{
            NextState = MCUSleep;
        }

    }
    else if(CurrentState == Activation){
        NextState = TempCycle;

    }
    else if(CurrentState == TempCycle){
        if(temperature >= 100){
            NextState = TempCycle;
        }
        else{
            NextState = MCUSleep;
        }
        //Sound Alarm is triggered by a timer interrupt.
        //If design needs to be changed to allow a direct path, then a flag can be created and checked here
        //if flag = 1, then go to alarm...
    }
    else if(CurrentState == SoundAlarm){
        NextState = SoundAlarm; //Stay in this state, when the alarm is silenced an interupt will be triggered that will set the state to disable alarm

    }
    else if(CurrentState == DisableAlarm){
        NextState = TempCycle;
    }
    else{ //important to have a default case but this case should never be reached. Maybe trigger a reset if this is hit?

    }

}





void OutputFunction(FSMStateType CurrentState)
{


}




