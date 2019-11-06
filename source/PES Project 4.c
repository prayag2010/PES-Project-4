/*
 * @file PES Project 4.c
 * @brief Source file that contains the entry point
 * of the application, which is the solution for
 * the PES Project 4
 *
 * This source file, meant for the FRDM-KL25Z board,
 * which is the solution for the PES Project 4,
 * implements two state machines, one table and
 * one state based. The state machines monitors
 * a TMP102 temperature sensor, connected via I2C, and
 * displays it's temperature, while recording an average.
 * The state machine changes the color of the onboard RGB
 * LED depending on the state entered. It alerts the user if
 * temperature has dropped below 0 C, while logging and checking
 * if the sensor is still connected.
 *
 * @authors Rahul Ramaprasad, Prayag Milan Desai
 * @date November 1 2019
 * @verison 1.0
 */


//All state functions are declared in this file
#include "stateFunctions.h"

//standard IO header for communication
#include <stdio.h>
//Logger messages
#include "loggerFunctions.h"

//Variables used for operation of the prog
#include "PESProject4.h"

//I2C communication functions
#include "i2c.h"

//RGB LED control functions
#include "ledControl.h"

//uC unit test
#include "unitTest.h"



int main(void)
{
	//Configure PD5 as a level triggered pulled up interrupt
	initPortD();

	//Initialize the systick timer to tick in 15 seconds
	Init_SysTick();

	//Initialize the i2c peripheral
	i2c_master_init();

	//Start communication with TMP102
	start();

	//Read temperature from the TMP102
	read_temp();

	//Initialize the RGB LEDs
	Init_RGB_LEDs();
	ledOff();

	//Turn the LEDs off
	currentState = tempRead;

	//Function pointer for the table based state machine
	enum eventCodes (* stateFunction)(void);

#ifdef test
	unitTest();
#endif

	//Infinite loop
	while (1) {

		//If the table based state machine is active
		if (stateTableActivated)
		{
			//Log
			log_message(DEBUG, __func__, "IN TABLE BASED");

			//Load the new state
			stateFunction = state[currentState];
			//Execute the state functions
			returnEvent = stateFunction();

			//If the table is activated, grab the next state from the
			//state table
			if(stateTableActivated)
				currentState = stateTable[currentState].onEventArray[returnEvent];
		}
		//If the state based state machine is active
		else
		{
			log_message(DEBUG, __func__, "IN STATE BASED");
			switch (currentState)
			{
			case tempRead:
				//Start reading temperature
				start();
				//Check for error
				if(read_temp() == 0xFFFF)
				{
					//disconnect if error
					currentState = disconnect;
					break;
				}
				//Execute the temperature read state
				tempReadState();
				//If temperature alert, address that
				if(!alertAddressed)
				{
					alertAddressed = true;
					//set next state
					currentState = tempAlert;
					break;
				}
				//set next state
				currentState = avgWait;
				break;
			case tempAlert:
				//Start reading temperature
				start();
				//Check for error
				if(read_temp() == 0xFFFF)
				{
					currentState = disconnect;
					break;
				}
				//Execute the temperature alert state
				tempAlertState();
				//set next state
				currentState = avgWait;
				break;
			case avgWait:
				//Start reading temperature
				start();
				//Check for error
				if(read_temp() == 0xFFFF)
				{
					currentState = disconnect;
					break;
				}
				//Execute the  average and wait state
				avgWaitState();
				//set next state
				currentState = tempRead;
				break;
			case error:
				//If the FSM enters in an error state, execute this
				errorState();
				currentState = tempRead;
				break;
			case disconnect:
				//If the temperature sensor is not detected, go to
				//disconnect
				disconnectState();
				currentState = tempRead;
				break;
			default:
				//If the state machine enters an unknown state, go to tempRead
				printf("State based state machine state unknown, setting to tempRead\n");
				printf("The unknown state is: %d\n", currentState);
				currentState = tempRead;
				break;
			}
		}
	}

	return 0 ;

}


