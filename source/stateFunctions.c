/*
 * @file stateFunctions.c
 * @brief Source file that contains state functions
 *
 * This source file contains the functions used by the state
 * machines, as well as the enums used for keeping
 * track of the state and the events happening in the
 * state machine
 *
 * @authors Rahul Ramaprasad, Prayag Milan Desai
 * @date November 1 2019
 * @verison 1.0
 */

#include "stateFunctions.h"
#include "MKL25Z4.h"
#include <stdio.h>

//Count how many times systick interrupted
volatile int sysTickCounter = 0;

//Enable the logger
bool loggerEnable = true;

//The state machine function pointer array
enum eventCodes (* state[])(void) = {tempReadState, tempAlertState, avgWaitState, disconnectState, errorState};

//Check how manu timers timeout has happened
int timeoutCounter = 0;
//Check if state or table based machine is active
bool stateTableActivated = true;


//The state table, contains which state to enter next depending on the
//current state and the event that happened
//timeoutEvent, completeEvent, alertEvent, disconnectEvent, errorEvent
struct stateMachineSkeleton stateTable[] = {
    {{error, avgWait, tempAlert, disconnect, error}}, //tempReadState
    {{error, avgWait, error, disconnect, error}}, //tempAlertState
    {{tempRead, error, error, disconnect, error}}, //avgWaitState
    {{error, error, error, error, error}},//disconnectState
    {{avgWait, avgWait, avgWait, avgWait, avgWait}}//errorState
};

//The initial state
enum stateCodes currentState = tempRead;
//The return event from the state
enum eventCodes returnEvent;

//Average of temp
int average = 0;
//Sum of temp
int tempSum = 0;
//The temperature
uint16_t tempR = 0;

//Check if 15 secs passed
volatile bool delayCompleted = false;
//Check if the alert was addressed, if it happened
volatile bool alertAddressed = true;



//A small delay for the interrupt to settle
void smallDelay(void)
{
	for(int i = 0; i < 10000; i++);
}

//If program ends
void endProgram(void)
{
	//reset systick timer
	resetSysTick();
	//disable interrupts
	__disable_irq();
	//turn the red LED on
	ledOff();
	redLED();
//	printf("Temperature sensor error\n");
//	printf("Ending Program\n");
	log_message(DEBUG, __func__, "Temperature sensor error, Ending Program");
	log_message(TEST, __func__, "Temperature sensor error, Ending Program");
	log_message(NORMAL, __func__, "Temperature sensor error, Ending Program");

	while(1);
}

enum eventCodes tempReadState(void)
{
    UCUNIT_Tracepoint(0); /* mark trace point */

	log_message(0, __func__, "Entered Temperature reading state");

	//Enable port D interrupt for alert
	PORTD->PCR[5] |= PORT_PCR_ISF_MASK;
	NVIC->ISER[0] |= (1 << PORTD_IRQn);

	//small delay for intr to settle
	smallDelay();

	//turn on the green LED
	ledOff();
	greenLED();

	//Take the temperature reading
	start();

	//return disconnect if error
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
		log_message(DEBUG, __func__, "Alert Detected");
		log_message(TEST, __func__, "Alert Detected");
		log_message(NORMAL, __func__, "Alert Detected");
		return alertEvent;
	}

	return completeEvent;
}

enum eventCodes tempAlertState(void)
{
    UCUNIT_Tracepoint(1); /* mark trace point */

	//disable port D interrupt
	log_message(DEBUG, __func__, "Entered Alert state");
	NVIC->ICER[0] |= (1 << PORTD_IRQn);

	ledOff();
	blueLED();

	//return disconnect if error
	start();
	if(read_temp() == 0xFFFF)
		return disconnectEvent;

//	printf("tempAlertState\n");

	if(!alertAddressed)
	{
		alertAddressed = true;
//		printf("ALERT DETECTED\n");
		log_message(DEBUG, __func__, "Alert Detected");
		log_message(TEST, __func__, "Alert Detected");
		log_message(NORMAL, __func__, "Alert Detected");
	}

	return completeEvent;
}

enum eventCodes avgWaitState(void)
{
    UCUNIT_Tracepoint(2); /* mark trace point */

	log_message(DEBUG, __func__, "Entered Average Wait State, disabling PORTD IRQ");
//	printf("Entered avgWaitState, disabling PORTD IRQ\n");
	NVIC->ICER[0] |= (1 << PORTD_IRQn);

	ledOff();
	greenLED();

	//return disconnect if error
	start();
	if(read_temp() == 0xFFFF)
		return disconnectEvent;

	//start 15 sec delay
	resetSysTick();
	startSysTick();

	//calc total
	if(negative)
		tempSum -= (int)tempR;
	else
		tempSum += (int)tempR;

	//calc average
	average = tempSum / (timeoutCounter + 1);
	printf("Average: %d\n", average);

	//wait for delay
	while(!delayCompleted);
	printf("avgWaitState\n");
	timeoutCounter++;
	printf("Counter: %d\n", timeoutCounter);


	//If counter runs 4th time, switch state machine
	if (timeoutCounter >= 3)
	{
		tempSum = 0;
		average = 0;
//		printf("Switched to a different state machine\n");
		log_message(DEBUG, __func__, "Switched to a different state machine");
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
    UCUNIT_Tracepoint(3); /* mark trace point */

	//disable interrupts
	log_message(DEBUG, __func__, "Entered Disconnect state");
	NVIC->ICER[0] |= (1 << PORTD_IRQn);
	printf("disconnectState\n");
	//end the program
#ifndef test
	endProgram();
#endif
	return errorEvent;
}

enum eventCodes errorState(void)
{
    UCUNIT_Tracepoint(4); /* mark trace point */

	//disable port D interrupt
	log_message(DEBUG, __func__, "Entered Error state");
	NVIC->ICER[0] |= (1 << PORTD_IRQn);
//	printf("errorState\n");
//	printf("State machine return value error, going back to getTemp state\n");
	log_message(DEBUG, __func__, "State machine return value error, going back to getTemp state");
	log_message(TEST, __func__, "State machine return value error, going back to getTemp state");
	log_message(NORMAL, __func__, "State machine return value error, going back to getTemp state");
	//go back to average wait
	return completeEvent;
}


//Initialize the PD5 pin to GPIO high level based interrupt
void initPortD(void)
{
	//enable port D clock
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;

	//Initialize the PD5 pin to GPIO high level based interrupt
	PORTD->PCR[5] = PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_MUX(1) | \
			PORT_PCR_IRQC(0xC);

	//Clear the interrupt mask
	PORTD->PCR[5] |= PORT_PCR_ISF_MASK;
	//Enable port D interrupt
	NVIC->ISER[0] |= (1 << PORTD_IRQn);
}

//Reset the systick timer
void resetSysTick(void)
{
	//Disable the systick
	SysTick->CTRL &= ~(SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk);
	//Load value eq to 15 sec
	SysTick->LOAD = sec3;
	//set the value register to 0
	SysTick->VAL = 0;
	//reset flag
	delayCompleted = false;
}

//start the systick timer
void startSysTick(void)
{
	//Enable and start the timer
	SysTick->CTRL = SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}


void Init_SysTick(void)
{
	//Load value eq to 15 sec
	SysTick->LOAD = sec3;
	//Set the systick timer priority
	NVIC_SetPriority(SysTick_IRQn, 3);
	//set the value register to 0
	SysTick->VAL = 0;
}


void PORTD_IRQHandler(void)
{
	//Clear the interrupt mask
	PORTD->PCR[5] |= PORT_PCR_ISF_MASK;
	//disable the interrupt
	NVIC->ICER[0] |= (1 << PORTD_IRQn);

	printf("Entered port interrupt, disabling IRQ\n");
	printf("ALERT ALERT ALERT ALERT ALERT ALERT\n");
	//set user mask
	alertAddressed = false;

}

void SysTick_Handler(void)
{
	//increment counter variable
	sysTickCounter++;
	//execute if the timer is used twice
	if(sysTickCounter > 1)
	{
		//reset counter
		sysTickCounter = 0;
		printf("SYSTICK, 15 seconds completed\n");
		//set flags
		delayCompleted = true;
	}
}
