/*
 * ledControl.h
 *
 *  Created on: Nov 3, 2019
 *      Author: rahul
 */

#ifndef LEDCONTROL_H_
#define LEDCONTROL_H_

#include "../CMSIS/MKL25Z4.h"

#define RED_LED_POS (18)  //Define port number for Red LED
#define GREEN_LED_POS (19) //Define port number for Green LED
#define BLUE_LED_POS (1) //Define port number for Blue LED

#define MASK(x) (1UL << (x))

void Init_RGB_LEDs(void);
void ledOff();

void redLED();

void greenLED();

void blueLED();

#endif /* LEDCONTROL_H_ */
