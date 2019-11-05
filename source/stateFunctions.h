/*
 * @file stateFunctions.h
 * @brief Header file that contains function and variable declarations
 *
 * This header file has the function prototypes for the
 * state machines and the ISR used by the program
 *
 * @authors Rahul Ramaprasad, Prayag Milan Desai
 * @date November 1 2019
 * @verison 1.0
 */

#ifndef STATEFUNCTIONS_H_
#define STATEFUNCTIONS_H_

#include "i2c.h"

//Look here for the state machine function prototypes
//and description
#include "PESProject4.h"
#include <stdint.h>
#include <stdbool.h>


/*
 * @brief Delay a certain amount of time
 *
 * This function delays bu having a for loop
 * for a certain amount of time, for the
 * interrupt to settle
 *
 * @return void
 */
void smallDelay(void);

/*
 * @brief End program
 *
 * This function is executed in case something
 * goes wrong, and it terminates the program by
 * going in a while loop and showing the red status LED
 *
 * @return void
 */
void endProgram(void);

//enum eventCodes tempReadState(void);
//
//enum eventCodes tempAlertState(void);
//
//enum eventCodes avgWaitState(void);
//
//enum eventCodes disconnectState(void);
//
//enum eventCodes errorState(void);

//void initPortD(void);

//void resetSysTick(void);
//
//void startSysTick(void);
//
//void Init_SysTick(void);


/*
 * @brief Port D interrupt handler
 *
 * This function resets the interrupt mask for
 * PD5, and sets user flags for alert
 *
 * @return void
 */
void PORTD_IRQHandler(void);


/*
 * @brief Systick timer interrupt handler
 *
 * This function counts the number of time
 * systick has interrupted, and gives a delay
 * of 15 sec depending on that counter
 *
 * @return void
 */
void SysTick_Handler(void);


#endif /* STATEFUNCTIONS_H_ */
