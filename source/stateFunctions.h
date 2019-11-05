/*
 * stateFunctions.h
 *
 *  Created on: 05-Nov-2019
 *      Author: prayag
 */

#ifndef STATEFUNCTIONS_H_
#define STATEFUNCTIONS_H_

#include "i2c.h"
#include "PESProject4.h"
#include <stdint.h>
#include <stdbool.h>



void smallDelay(void);

void endProgram(void);

enum eventCodes tempReadState(void);

enum eventCodes tempAlertState(void);

enum eventCodes avgWaitState(void);

enum eventCodes disconnectState(void);

enum eventCodes errorState(void);

void initPortD(void);

void resetSysTick(void);

void startSysTick(void);

void Init_SysTick(void);

void PORTD_IRQHandler(void);

void SysTick_Handler(void);


#endif /* STATEFUNCTIONS_H_ */
