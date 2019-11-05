/*
 * stateFunctions.c
 *
 *  Created on: 05-Nov-2019
 *      Author: prayag
 */
#include "stateFunctions.h"
#include "ledControl.h"
#include "MKL25Z4.h"
#include <stdio.h>




volatile int sysTickCounter = 0;

bool loggerEnable = true;




enum eventCodes (* state[])(void) = {tempReadState, tempAlertState, avgWaitState, disconnectState, errorState};

int timeoutCounter = 0;
bool stateTableActivated = true;



//timeoutEvent, completeEvent, alertEvent, disconnectEvent, errorEvent
struct stateMachineSkeleton stateTable[] = {
    {{error, avgWait, tempAlert, disconnect, error}}, //tempReadState
    {{error, avgWait, error, disconnect, error}}, //tempAlertState
    {{tempRead, error, error, disconnect, error}}, //avgWaitState
    {{error, error, error, error, error}},//disconnectState
    {{avgWait, avgWait, avgWait, avgWait, avgWait}}//errorState
};

enum stateCodes currentState = tempRead;
enum eventCodes returnEvent;

int average = 0;
int tempSum = 0;
uint16_t tempR = 0;

volatile bool delayCompleted = false;
volatile bool alertAddressed = true;






void smallDelay(void)
{
	for(int i = 0; i < 10000; i++);
}

void endProgram(void)
{
	resetSysTick();
	__disable_irq();
	ledOff();
	redLED();
//	printf("Temperature sensor error\n");
//	printf("Ending Program\n");
	log_message(0, __func__, "Temperature sensor error, Ending Program");
	log_message(1, __func__, "Temperature sensor error, Ending Program");
	log_message(2, __func__, "Temperature sensor error, Ending Program");

	while(1);
}

enum eventCodes tempReadState(void)
{
	log_message(0, __func__, "Entered Temperature reading state");
	PORTD->PCR[5] |= PORT_PCR_ISF_MASK;
	NVIC->ISER[0] |= (1 << PORTD_IRQn);
	//	initPortD();
	smallDelay();

	ledOff();
	greenLED();

	start();
	tempR = read_temp();
	if(tempR == 0xFFFF)
		return disconnectEvent;

	if (!(negative))
		printf("tempReadState %d\n", tempR);
	else
	{
		printf("tempReadState -%d\n", tempR);
		negative = false;
	}

	if(!alertAddressed)
	{
		//		alertAddressed = true;
//		printf("ALERT DETECTED\n");
		log_message(0, __func__, "Alert Detected");
		log_message(1, __func__, "Alert Detected");
		log_message(2, __func__, "Alert Detected");
		return alertEvent;
	}

	return completeEvent;
}

enum eventCodes tempAlertState(void)
{
	log_message(0, __func__, "Entered Alert state");
	NVIC->ICER[0] |= (1 << PORTD_IRQn);

	ledOff();
	blueLED();

	start();
	if(read_temp() == 0xFFFF)
		return disconnectEvent;

//	printf("tempAlertState\n");

	if(!alertAddressed)
	{
		alertAddressed = true;
//		printf("ALERT DETECTED\n");
		log_message(0, __func__, "Alert Detected");
		log_message(1, __func__, "Alert Detected");
		log_message(2, __func__, "Alert Detected");
	}

	return completeEvent;
}

enum eventCodes avgWaitState(void)
{
	log_message(0, __func__, "Entered Average Wait State, disabling PORTD IRQ");
//	printf("Entered avgWaitState, disabling PORTD IRQ\n");
	NVIC->ICER[0] |= (1 << PORTD_IRQn);

	ledOff();
	greenLED();

	start();
	if(read_temp() == 0xFFFF)
		return disconnectEvent;


	resetSysTick();
	startSysTick();

	if(negative)
		tempSum -= (int)tempR;
	else
		tempSum += (int)tempR;

	average = tempSum / (timeoutCounter + 1);
	printf("Average: %d\n", average);

	while(!delayCompleted);
	printf("avgWaitState\n");
	timeoutCounter++;
	printf("Counter: %d\n", timeoutCounter);


	if (timeoutCounter >= 3)
	{
		tempSum = 0;
		average = 0;
//		printf("Switched to a different state machine\n");
		log_message(0, __func__, "Switched to a different state machine");
		timeoutCounter = 0;
		currentState = tempRead;
		stateTableActivated = !stateTableActivated;
		return timeoutEvent;
	}
	else
		return timeoutEvent;

}

enum eventCodes disconnectState(void)
{
	log_message(0, __func__, "Entered Disconnect state");
	NVIC->ICER[0] |= (1 << PORTD_IRQn);
	printf("disconnectState\n");
	endProgram();
	return errorEvent;
}

enum eventCodes errorState(void)
{
	log_message(0, __func__, "Entered Error state");
	NVIC->ICER[0] |= (1 << PORTD_IRQn);
//	printf("errorState\n");
//	printf("State machine return value error, going back to getTemp state\n");
	log_message(0, __func__, "State machine return value error, going back to getTemp state");
	log_message(1, __func__, "State machine return value error, going back to getTemp state");
	log_message(2, __func__, "State machine return value error, going back to getTemp state");
	return completeEvent;
}
