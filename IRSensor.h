/*
 * IRSensor.h
 *
 *  Created on: Oct 13, 2021
 *      Author: tyler
 */

#ifndef IRSENSOR_H_
#define IRSENSOR_H_

#include <stdint.h>

void initIRSensor();
//void convertToF(uint16_t data);
uint16_t readTemperature();


#endif /* IRSENSOR_H_ */
