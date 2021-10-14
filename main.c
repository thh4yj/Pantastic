/**
 * main.c
 */
#include "FSM.h"
#include "IRSensor.h"
#include "Piezzo.h"

// ### DUMMY HEADERS ###
void init_Wifi();
void init_LEDMatrix();
void updateLEDs(uint16_t data); // Might need to change data value later
void sendTempToWiFi(uint16_t temperature); //Prereq is that temp is already in degress Fahrenheit
void initButtons();
void readButtons();
//uint16_t readTemperature();
// ### END DUMMY HEADERS ###

int main(void)
{
    //initialize drivers
    initIRSensor();
    initPiezzo();
    //******

    //Setup needed values
    FSMStateType CurrentState = Init;
    uint16_t temperature = 0;
    while(1){
        temperature = readTemperature();
        CurrentState = NextStateFunction(CurrentState, temperature);
        OutputFunction(CurrentState);
    }
	return 0;
}
