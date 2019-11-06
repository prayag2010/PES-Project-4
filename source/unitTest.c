/*
 * @file unitTest.c
 * @brief Source file has all the unit tests
 *
 * This source file contains all the unit tests called, which are defined
 * in the uCUnit.h header
 *
 * @authors Rahul Ramaprasad, Prayag Milan Desai
 * @date November 1 2019
 * @verison 1.0
 */

#include "uCUnit.h"
#include <stdint.h>
#include "unitTest.h"

void unitTest(void)
{
	//Initialize testing
	UCUNIT_Init();
	UCUNIT_TestcaseBegin("Unit test on I2C and Temperature Sensor");

	//Checking if the power on self test works
	UCUNIT_WriteString("Checking if POST is passed\n");
	start();
	uint16_t temperature = read_temp();
	UCUNIT_CheckIsEqual(postCheck, true);

	//Checking current temperature
	UCUNIT_WriteString("Checking if the sensor is reading the current temperature\n");
	start();
	temperature = read_temp();
	UCUNIT_CheckIsEqual(temperature, 23);

	//Checking if the temperature sensor is disconnected
	UCUNIT_WriteString("Checking if the sensor is disconnected\n");
	start();
	temperature = read_temp();
	UCUNIT_CheckIsEqual(temperature, 0xFFFF);

	//Checking if the KL25Z is in I2C transmit mode
	UCUNIT_WriteString("Checking if the KL25Z is in I2C transmit mode\n");
	start();
	temperature = read_temp();
	UCUNIT_CheckIsEqual((I2C1->C1 & I2C_C1_TX_MASK), I2C_C1_TX_MASK);

	//Checking if the temperature variable 32 bit
	UCUNIT_WriteString("Checking if the temperature variable is 16 bit\n");
	UCUNIT_CheckIs16Bit(tempR);

	//Check the tracepoint coverage in tempRead state
	UCUNIT_WriteString("Checking the tracepoint in tempRead state\n");
	UCUNIT_ResetTracepointCoverage(); /* start tracking */
	tempReadState();
	UCUNIT_CheckTracepointCoverage(0);

	//Check the tracepoint coverage in Alert state
	UCUNIT_WriteString("Checking the tracepoint in Alert state\n");
	UCUNIT_ResetTracepointCoverage(); /* start tracking */
	tempAlertState();
	UCUNIT_CheckTracepointCoverage(1);

	//Check the tracepoint coverage in avgWaitState state
	UCUNIT_WriteString("Checking the tracepoint in avgWaitState state\n");
	UCUNIT_ResetTracepointCoverage(); /* start tracking */
	avgWaitState();
	UCUNIT_CheckTracepointCoverage(2);

//	//Check the tracepoint coverage in disconnected state
	UCUNIT_WriteString("Checking the tracepoint in disconnected state\n");
	UCUNIT_ResetTracepointCoverage(); /* start tracking */
	disconnectState();
	UCUNIT_CheckTracepointCoverage(3);

	//Check the tracepoint coverage in Error state
	UCUNIT_WriteString("Checking the tracepoint in Error state\n");
	//main();
	UCUNIT_ResetTracepointCoverage(); /* start tracking */
	errorState();
	UCUNIT_CheckTracepointCoverage(4);

	//Turn the status LED to green to indicated end of program
	ledOff();
	greenLED();
	//End testing and write summary
	UCUNIT_WriteSummary();
	UCUNIT_Shutdown();
}
