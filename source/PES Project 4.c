

#include <stdio.h>
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "loggerFunctions.h"
#include "PESProject4.h"
#include "i2c.h"
#include "ledControl.h"

volatile int i = 0;

int average = 0;
int tempSum = 0;
uint16_t tempR = 0;

bool loggerEnable = true;
volatile bool delayCompleted = false;
volatile bool alertAddressed = true;

#define sec15 42915344L
#define sec3 858306L

void smallDelay(void)
{
	for(int i = 0; i < 10000; i++);
}

void endProgram(void)
{
	__disable_irq();
	ledOff();
	redLED();
	printf("Temperature sensor error\n");
	printf("Ending Program\n");
	while(1);
}

enum eventCodes tempReadState(void)
{
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

	if(!alertAddressed){
//		alertAddressed = true;
		printf("ALERT DETECTED\n");
		return alertEvent;
	}

	return completeEvent;
}

enum eventCodes tempAlertState(void)
{
	NVIC->ICER[0] |= (1 << PORTD_IRQn);

	ledOff();
	blueLED();

	start();
	if(read_temp() == 0xFFFF)
		return disconnectEvent;

	printf("tempAlertState\n");

	if(!alertAddressed){
		alertAddressed = true;
		printf("ALERT DETECTED\n");
	}

	return completeEvent;
}

enum eventCodes avgWaitState(void)
{
	printf("Entered avgWaitState, disabling PORTD IRQ\n");
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
	printf("AverageAverageAverageAverageAverageAverageAverage: %d\n", average);

	while(!delayCompleted);
	printf("avgWaitState\n");
	timeoutCounter++;
	printf("Counter: %d\n", timeoutCounter);


	if (timeoutCounter >= 3) {
		tempSum = 0;
		average = 0;
		printf("Switched to a different state machine\n");
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
	NVIC->ICER[0] |= (1 << PORTD_IRQn);
	printf("disconnectState\n");
	endProgram();
	return errorEvent;
}

enum eventCodes errorState(void)
{
	NVIC->ICER[0] |= (1 << PORTD_IRQn);
	printf("errorState\n");
	printf("State machine return value error, going back to getTemp state\n");
	return completeEvent;
}


int main(void) {

	initPortD();
	Init_SysTick();
	i2c_master_init();
	start();
	read_temp();

	Init_RGB_LEDs();

	currentState = tempRead;
	enum eventCodes (* stateFunction)(void);



	while (1) {

		if (stateTableActivated) {
			printf("IN TABLE BASED\n");
			stateFunction = state[currentState];
			returnEvent = stateFunction();
			if(stateTableActivated)
				currentState = stateTable[currentState].onEventArray[returnEvent];
		} else {
			printf("IN STATE BASED\n");
			printf("Current state: %d\n", currentState);
			switch (currentState)
			{
			case tempRead:
				start();
				if(read_temp() == 0xFFFF){
					currentState = disconnect;
					break;
				}
				tempReadState();
				if(!alertAddressed){
					alertAddressed = true;
					printf("ALERT DETECTED\n");
					currentState = tempAlert;
					break;
				}
				currentState = avgWait;
				break;
			case tempAlert:
				start();
				if(read_temp() == 0xFFFF){
					currentState = disconnect;
					break;
				}
				tempAlertState();
				currentState = avgWait;
				break;
			case avgWait:
				start();
				if(read_temp() == 0xFFFF){
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

void SysTick_Handler(void)  {
	i++;
	if(i > 1){
		i = 0;
		printf("SYSTICK, 15 seconds completed\n");
		delayCompleted = true;
	}
}
