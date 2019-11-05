/*
 * PESProject4.h
 *
 *  Created on: 03-Nov-2019
 *      Author: prayag
 */

#ifndef PESPROJECT4_H_
#define PESPROJECT4_H_

#define sec15 42915344L
#define sec3 858306L


#include <stdbool.h>

enum eventCodes {timeoutEvent, completeEvent, alertEvent, disconnectEvent, errorEvent};
enum stateCodes {tempRead, tempAlert, avgWait, disconnect, error};

void Init_SysTick(void);
void initPortD(void);
enum eventCodes tempReadState(void);
enum eventCodes tempAlertState(void);
enum eventCodes avgWaitState(void);
enum eventCodes disconnectState(void);
enum eventCodes errorState(void);
void startSysTick(void);
void resetSysTick(void);


extern enum eventCodes (* state[])(void);

extern int timeoutCounter;
extern bool stateTableActivated;

struct stateMachineSkeleton {
    enum stateCodes onEventArray[5];
    // enum stateCodes onTimeoutEventState;
    // enum stateCodes onCompleteEventState;
    // enum stateCodes onAlertEventState;
    // enum stateCodes onDisconnectEventState;
};

//timeoutEvent, completeEvent, alertEvent, disconnectEvent, errorEvent
extern struct stateMachineSkeleton stateTable[5];
extern enum stateCodes currentState;
extern enum eventCodes returnEvent;

extern int average;
extern int tempSum;
extern uint16_t tempR;

extern volatile bool delayCompleted;
extern volatile bool alertAddressed;

extern volatile int sysTickCounter;



#endif /* PESPROJECT4_H_ */
