/*
 * ledControl.c
 *
 *  Created on: Nov 3, 2019
 *      Author: rahul
 */

#include "ledControl.h"

void Init_RGB_LEDs(void)
{
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;  //Enable Port B and Port D

	PORTB->PCR[RED_LED_POS] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED_POS] |= PORT_PCR_MUX(1);   //Enable the LED by selecting alternative function of the port

	PORTB->PCR[GREEN_LED_POS] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED_POS] |= PORT_PCR_MUX(1); //Enable the LED by selecting alternative function of the port

	PORTD->PCR[BLUE_LED_POS] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED_POS] |= PORT_PCR_MUX(1);  //Enable the LED by selecting alternative function of the port

	PTB->PDDR |= MASK(RED_LED_POS) | MASK(GREEN_LED_POS);
	PTD->PDDR |= MASK(BLUE_LED_POS);

}

void ledOff()
{
	PTB->PSOR = MASK(RED_LED_POS);  //Switch off RED
	PTB->PSOR =MASK(GREEN_LED_POS); //Switch off GREEN
	PTD->PSOR = MASK(BLUE_LED_POS); //Switch off BLUE
}

void redLED()
{
	ledOff();
	PTB->PCOR = MASK(RED_LED_POS); //Switch on RED
}

void greenLED()
{
	ledOff();
	PTB->PCOR = MASK(GREEN_LED_POS); //Switch on GREEN
}

void blueLED()
{
	ledOff();
	PTD->PCOR = MASK(BLUE_LED_POS); //Switch on BLUE
}
