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
 * temperature has dropped below 0, while logging and checking
 * if the sensor is still connected.
 *
 * @authors Rahul Ramaprasad, Prayag Milan Desai
 * @date November 1 2019
 * @verison 1.0
 */


//All state functions are declared in this file
#include "stateFunctions.h"

#include <stdio.h>
//#include "board.h"
//#include "pin_mux.h"
//#include "clock_config.h"

//Logger messages
#include "loggerFunctions.h"

//Variables used for operation of the prog
#include "PESProject4.h"

//I2C communication functions
#include "i2c.h"

//RGB LED control functions
#include "ledControl.h"



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

	//Infinite loop
	while (1) {

		//If the table based state machine is active
		if (stateTableActivated)
		{
//			printf("IN TABLE BASED\n");
			//Log
			log_message(0, __func__, "IN TABLE BASED");
			log_message(1, __func__, "IN TABLE BASED");
			log_message(2, __func__, "IN TABLE BASED");

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
//			printf("IN STATE BASED\n");
			log_message(0, __func__, "IN STATE BASED");
			log_message(1, __func__, "IN STATE BASED");
			log_message(2, __func__, "IN STATE BASED");
			printf("Current state: %d\n", currentState);
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
					printf("ALERT DETECTED\n");
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
