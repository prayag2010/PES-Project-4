/*
 * PESProject4.h
 *
 *  Created on: 03-Nov-2019
 *      Author: prayag
 */

#ifndef PESPROJECT4_H_
#define PESPROJECT4_H_

void Init_SysTick(void);
void initPortD(void);

enum eventCodes {timeoutEvent, completeEvent, alertEvent, disconnectEvent, errorEvent};

enum eventCodes tempReadState(void);
enum eventCodes tempAlertState(void);
enum eventCodes avgWaitState(void);
enum eventCodes disconnectState(void);
enum eventCodes errorState(void);
void startSysTick(void);
void resetSysTick(void);


enum stateCodes {tempRead, tempAlert, avgWait, disconnect, error};
enum eventCodes (* state[])(void) = {tempReadState, tempAlertState, avgWaitState, disconnectState, errorState};

int timeoutCounter = 0;
bool stateTableActivated = true;

struct stateMachineSkeleton {
    enum stateCodes onEventArray[5];
    // enum stateCodes onTimeoutEventState;
    // enum stateCodes onCompleteEventState;
    // enum stateCodes onAlertEventState;
    // enum stateCodes onDisconnectEventState;
};

//timeoutEvent, completeEvent, alertEvent, disconnectEvent, errorEvent
struct stateMachineSkeleton stateTable[] = {
    {error, avgWait, tempAlert, disconnect, error}, //tempReadState
    {error, avgWait, error, disconnect, error}, //tempAlertState
    {tempRead, error, error, disconnect, error}, //avgWaitState
    {error, error, error, error, error},//disconnectState
    {avgWait, avgWait, avgWait, avgWait, avgWait}//errorState
};

enum stateCodes currentState = tempRead;
enum eventCodes returnEvent;



#endif /* PESPROJECT4_H_ */
