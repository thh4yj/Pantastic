/*
 * FSM.c
 *
 *  Created on: Oct 13, 2021
 *      Author: tyler
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
    //if the current state is stop then the LED will turn off.
    if(CurrentState == Stop){
        LaunchPad_LED(LED_OFF);
    }
    else{
        H = PulseBuf[i]; //set how long the LED should be on
        L = 10000-H; //set how long the LED should be off
     LaunchPad_LED(LED_ON); //turn on the LED
     SysTick_Wait1us(H); //Wait high
     LaunchPad_LED(LED_OFF); //Turn off the LED
     SysTick_Wait1us(L); //Wait low
     i = (i+1)%100; //increment or reset i to prevent an index out of bounds error and to keep the cycle running

    }

}




