

#include <stdio.h>
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "loggerFunctions.h"
#include "PESProject4.h"
#include "i2c.h"


volatile int i = 0;

bool loggerEnable = true;
volatile bool delayCompleted = false;
volatile bool alertAddressed = true;

void smallDelay(void)
{
	for(int i = 0; i < 10000; i++);
}

enum eventCodes tempReadState(void)
{
	PORTD->PCR[5] |= PORT_PCR_ISF_MASK;
	PORTD->PCR[5] = PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_MUX(1) | \
				PORT_PCR_IRQC(0x8);
//	initPortD();
	smallDelay();
	i2c_master_init();
	read_temp();
	printf("tempReadState\n");
	return alertEvent;
}

enum eventCodes tempAlertState(void)
{
	NVIC->ICER[0] |= (1 << PORTD_IRQn);
	printf("tempAlertState\n");
	return completeEvent;
}

enum eventCodes avgWaitState(void)
{
	printf("Entered avgWaitState, disabling IRQ\n");
	NVIC->ICER[0] |= (1 << PORTD_IRQn);
	resetSysTick();
	startSysTick();

	while(!delayCompleted);
	printf("avgWaitState\n");
	timeoutCounter++;
	printf("Counter: %d\n", timeoutCounter);

	if (timeoutCounter >= 3) {
		printf("Switched to a different state\n");
		timeoutCounter = 0;
		currentState = tempRead;
		stateTableActivated = !stateTableActivated;
		return errorEvent;
	}
	else
		return timeoutEvent;

}

enum eventCodes disconnectState(void)
{
	NVIC->ICER[0] |= (1 << PORTD_IRQn);
	printf("disconnectState\n");
	printf("END HERE\n");
	return errorEvent;
}

enum eventCodes errorState(void)
{
	NVIC->ICER[0] |= (1 << PORTD_IRQn);
	printf("errorState\n");
	return disconnectEvent;
}


int main(void) {

	initPortD();
	Init_SysTick();
	i2c_master_init();
	read_temp();

	currentState = tempRead;
	enum eventCodes (* stateFunction)(void);



	while (1) {

		if (stateTableActivated) {
			printf("IN TABLE BASED\n");
			stateFunction = state[currentState];
			returnEvent = stateFunction();
			currentState = stateTable[currentState].onEventArray[returnEvent];
		} else {
			printf("IN STATE BASED\n");
			switch (currentState)
			{
			case tempRead:
				tempReadState();
				currentState = tempAlert;
				break;
			case tempAlert:
				tempAlertState();
				currentState = avgWait;
				break;
			case avgWait:
				avgWaitState();
				currentState = tempRead;
				break;
			default:
				printf("State based state machine state unknown, setting to tempRead\n");
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
			PORT_PCR_IRQC(0x8);

	NVIC->ISER[0] |= (1 << PORTD_IRQn);
}

void resetSysTick(void)
{
	SysTick->CTRL &= ~(SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk);
	SysTick->LOAD = (42915344L);
	SysTick->VAL = 0;
	delayCompleted = false;
}

void startSysTick(void)
{
	SysTick->CTRL = SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}


void Init_SysTick(void)
{
	SysTick->LOAD = (42915344L);
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

void SysTick_Handler(void)  {
	i++;
	if(i > 1){
		i = 0;
		printf("SYSTICK, 15 seconds completed\n");
		delayCompleted = true;
	}
}
