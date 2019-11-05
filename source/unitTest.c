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
#include "i2c.h"
#include "unitTest.h"


void unitTest(void)
{
	//Initialize testing
	UCUNIT_Init();
	UCUNIT_TestcaseBegin("Unit test on I2C and Temperature Sensor");

	//Checking if the power on self test works
	UCUNIT_WriteString("Checking if POST is passed\n");
	i2c_master_init();
	UCUNIT_CheckIsEqual(postCheck, true);

	//Checking current temperature
	UCUNIT_WriteString("Checking if the sensor is reading the current temperature\n");
	i2c_master_init();
	uint16_t temperature = read_temp();
	UCUNIT_CheckIsEqual(temperature, 23);

	//Checking if the temperature sensor is disconnected
	UCUNIT_WriteString("Checking if the sensor is disconnected\n");
	i2c_master_init();
	temperature = read_temp();
	UCUNIT_CheckIsEqual(temperature, 0xFFFF);

	//Turn the status LED to green to indicated end of program


	//End testing and write summary
	UCUNIT_WriteSummary();
	UCUNIT_Shutdown();
}
