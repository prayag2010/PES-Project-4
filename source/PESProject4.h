/*
 * @file PESProject4.h
 * @brief Header file that contains function and variable declarations
 *
 * This header file has the function prototypes for the
 * state machines and the ISR used by the program, as well as the
 * event and state codes needed for operation
 *
 * @authors Rahul Ramaprasad, Prayag Milan Desai
 * @date November 1 2019
 * @verison 1.0
 */

#ifndef PESPROJECT4_H_
#define PESPROJECT4_H_

//Systick value for 15 seconds
#define sec15 42915344L

//Systick value for 3 seconds (for debugging)
#define sec3 858306L

//standard boolean header file
#include <stdbool.h>

//This enum lists all possible events that can happen in the state machine
enum eventCodes {timeoutEvent, completeEvent, alertEvent, disconnectEvent, errorEvent};

//This enum all the states that the state machine has
enum stateCodes {tempRead, tempAlert, avgWait, disconnect, error};


/*
 * @brief Initialize the systick timer
 *
 * This function initializes the systick timer to
 * tick at every 7.5 seconds, which is used twice for a
 * delay of 15 sec
 *
 * @return void
 */
void Init_SysTick(void);

/*
 * @brief Init port D
 *
 * This function initializes the PD5 pin to
 * GPIO high level based interrupt
 *
 * @return void
 */
void initPortD(void);


/*
 * @brief Temperature read state
 *
 * This reads the temperature form the TMP102
 * and returns an event code depending on the
 * success or failure of the same
 *
 * @return The event code depending on what the state
 * executes (complete, disconnect, etc)
 */
enum eventCodes tempReadState(void);


/*
 * @brief Temperature alert state
 *
 * This state is entered if a temperature
 * alert is detected. It changes the LED
 * to blue and alerts the user
 *
 * @return The event code depending on what the state
 * executes (complete, disconnect, etc)
 */
enum eventCodes tempAlertState(void);


/*
 * @brief Average wait state
 *
 * This state averages the read temperature from
 * the sensor, waits for 15 secs, and switches state
 * machines every 4th time
 *
 * @return The event code depending on what the state
 * executes (complete, disconnect, etc)
 */
enum eventCodes avgWaitState(void);


/*
 * @brief Disconnect state
 *
 * This state is entered if no temperature
 * sensor is connected, and it informs the user
 * the same before ending the program
 *
 * @return The event code depending on what the state
 * executes (complete, disconnect, etc)
 */
enum eventCodes disconnectState(void);

/*
 * @brief Error state
 *
 * This state is entered if an incorrect
 * transition is detected in the state machine
 * It resets the machine back to tempRead state
 *
 * @return The event code depending on what the state
 * executes (complete, disconnect, etc)
 */
enum eventCodes errorState(void);


/*
 * @brief Start the systick timer
 *
 * This function starts the systick timer for an
 * interval of 7.5 sec
 *
 * @return void
 */
void startSysTick(void);

/*
 * @brief Reset the systick timer
 *
 * This function resets the systick timer
 * to it's initial state
 *
 * @return void
 */
void resetSysTick(void);


//The function pointer that executes the state function
extern enum eventCodes (* state[])(void);

//See if the systick has timed out
extern int timeoutCounter;
//If the state or the table based state machine is activated
extern bool stateTableActivated;

//The state table declaration
struct stateMachineSkeleton {
    enum stateCodes onEventArray[5];
};

//timeoutEvent, completeEvent, alertEvent, disconnectEvent, errorEvent
//The state table
extern struct stateMachineSkeleton stateTable[5];

//The current state code
extern enum stateCodes currentState;
//The return event
extern enum eventCodes returnEvent;

//The average of temperatures
extern int average;
//The sum of the temperatures
extern int tempSum;
//Average denominator
extern int averageDiv;
//The read temperature from the sensor
extern uint16_t tempR;

//Check if 15 seconds have passed
extern volatile bool delayCompleted;

//Check if alert is addressed, in case it happened
extern volatile bool alertAddressed;

//Count the systick interrupts
extern volatile int sysTickCounter;



#endif /* PESPROJECT4_H_ */
