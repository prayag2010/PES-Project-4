
#include "stateFunctions.h"
#include <stdio.h>
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "loggerFunctions.h"
#include "PESProject4.h"
#include "i2c.h"
#include "ledControl.h"



int main(void)
{

	initPortD();
	Init_SysTick();
	i2c_master_init();
	start();
	read_temp();

	Init_RGB_LEDs();
	ledOff();
	currentState = tempRead;
	enum eventCodes (* stateFunction)(void);



	while (1) {

		if (stateTableActivated)
		{
//			printf("IN TABLE BASED\n");
			log_message(0, __func__, "IN TABLE BASED");
			log_message(1, __func__, "IN TABLE BASED");
			log_message(2, __func__, "IN TABLE BASED");
			stateFunction = state[currentState];
			returnEvent = stateFunction();
			if(stateTableActivated)
				currentState = stateTable[currentState].onEventArray[returnEvent];
		}
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
				start();
				if(read_temp() == 0xFFFF)
				{
					currentState = disconnect;
					break;
				}
				tempReadState();
				if(!alertAddressed)
				{
					alertAddressed = true;
					printf("ALERT DETECTED\n");
					currentState = tempAlert;
					break;
				}
				currentState = avgWait;
				break;
			case tempAlert:
				start();
				if(read_temp() == 0xFFFF)
				{
					currentState = disconnect;
					break;
				}
				tempAlertState();
				currentState = avgWait;
				break;
			case avgWait:
				start();
				if(read_temp() == 0xFFFF)
				{
					currentState = disconnect;
					break;
				}
				avgWaitState();
				currentState = tempRead;
				break;
			case error:
				errorState();
				currentState = tempRead;
				break;
			case disconnect:
				disconnectState();
				currentState = tempRead;
				break;
			default:
				printf("State based state machine state unknown, setting to tempRead\n");
				printf("The unknown state is: %d\n", currentState);
				currentState = tempRead;
				break;
			}
		}
	}

	return 0 ;

}

void initPortD(void)
{
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;

	PORTD->PCR[5] = PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_MUX(1) | \
			PORT_PCR_IRQC(0xC);

	PORTD->PCR[5] |= PORT_PCR_ISF_MASK;
	NVIC->ISER[0] |= (1 << PORTD_IRQn);
}

void resetSysTick(void)
{
	SysTick->CTRL &= ~(SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk);
	SysTick->LOAD = sec3;
	SysTick->VAL = 0;
	delayCompleted = false;
}

void startSysTick(void)
{
	SysTick->CTRL = SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}


void Init_SysTick(void)
{
	SysTick->LOAD = sec3;
	NVIC_SetPriority(SysTick_IRQn, 3);
	SysTick->VAL = 0;
}


void PORTD_IRQHandler(void)
{
	PORTD->PCR[5] |= PORT_PCR_ISF_MASK;
	NVIC->ICER[0] |= (1 << PORTD_IRQn);

	printf("Entered port interrupt, disabling IRQ\n");
	printf("ALERT ALERT ALERT ALERT ALERT ALERT\n");
	alertAddressed = false;

}

void SysTick_Handler(void)
{
	sysTickCounter++;
	if(sysTickCounter > 1)
	{
		sysTickCounter = 0;
		printf("SYSTICK, 15 seconds completed\n");
		delayCompleted = true;
	}
}
