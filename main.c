/**
 * main.c
 */
#include "FSM.h"
#include "IRSensor.h"
#include "Piezzo.h"
#include <ti/drivers/timer/TimerCC32XX.h>

//TODO
// 1. Learn how to implement GPIO on the pins so that we can send and receive signals
// 2. Learn how to use the Timer module so that we can run the unit based on time
// 3. Learn how to use interrupts so that we can trigger actions in real time
// 4. Learn how to use SMBus so that we can read the data from our sensor


// ### DUMMY HEADERS ###
void init_Wifi();
void init_LEDMatrix();
void updateLEDs(uint16_t data); // Might need to change data value later
void sendTempToWiFi(int16_t temperature); //Prereq is that temp is already in degress Fahrenheit
void initButtons();
void readButtons();
//uint16_t readTemperature();
// ### END DUMMY HEADERS ###

// ### Global Setup Variables ###
int16_t temperature = 0;

// ### End Setup ###

/*
 * Function to pass into timer to run when the IRQ handler executes
 * Should trigger at desired read rate, every 15 seconds when operational.
 * Function updates global temperature value for use within the FSM
 */
void Task(){

}

int main(void)
{
    //initialize drivers
    initIRSensor();
    initPiezzo();
    //******

    //Setup needed values
    FSMStateType CurrentState = Init;

    while(1){
        CurrentState = NextStateFunction(CurrentState, temperature);
        OutputFunction(CurrentState);
    }
	return 0;
}
