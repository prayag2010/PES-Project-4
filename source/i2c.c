/*
 * @file i2c.c
 * @brief Source file for controlling the I2C registers
 *
 * This header file has functions that initializes the
 * I2C and connects to the TMP102 temperature sensor,
 * which then returns the current temperature and the alert
 * status
 *
 * @authors Rahul Ramaprasad, Prayag Milan Desai
 * @date November 1 2019
 * @verison 1.0
 */

#include "../CMSIS/MKL25Z4.h"
#include <stdio.h>
#include "i2c.h"
#include "loggerFunctions.h"

//Function that terminates program
void endProgram(void);

//Set up tlow and thigh on boot
bool setupOnce = false;

//if temperature is -ve
bool negative = false;

//is POST passed
bool postCheck = false;

void i2c_master_init()
{

	SIM->SCGC4 |= SIM_SCGC4_I2C1_MASK;  //Enabling the clock for I2C module
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	PORTE->PCR[0] |= PORT_PCR_MUX(6) | 0x03;   //SCL
	PORTE->PCR[1] |= PORT_PCR_MUX(6) | 0x03;   //SDA

	I2C1->F |= I2C_F_MULT(0) | I2C_F_ICR(0x11);   //Setting a baud rate of 300kbps
	I2C1->C1|= I2C_C1_IICEN_MASK;// | I2C_C1_IICIE_MASK; //Enabling I2C module

	log_message(DEBUG, __func__, "I2C registers initialised");
	log_message(TEST, __func__, "I2C registers initialised");
}

void start()
{
	I2C1->S |= I2C_S_RXAK_MASK;
	I2C1->C1 |= I2C_C1_TXAK_MASK;

	I2C1->C1 |= I2C_C1_TX_MASK;  //Enabling transmit mode
	START;
	DATA(TMP102_WRITE);  //Transmit first byte
	WAIT;
	if((I2C1->S & I2C_S_RXAK_MASK) == 0)           //Check if slave received a byte
	{
		postCheck = true;
//		printf("Temperature sensor detected\n");
		log_message(DEBUG, __func__, "Temperature Sensor detected");
		log_message(TEST, __func__, "Temperature Sensor detected");
//		log_message(NORMAL, __func__, "Temperature Sensor detected");
		I2C1->S |= I2C_S_RXAK_MASK;
	}
	else{
//		printf("No device found\n");
		log_message(DEBUG, __func__, "No device found");
		log_message(TEST, __func__, "No device found");
		log_message(NORMAL, __func__, "No device found");
		endProgram();
	}

	//		disconnect();

	if(!setupOnce){
		//		RESTART;
		//		DATA(TMP102_WRITE);   //Transmit first byte
		//		WAIT;
		//		DATA(0x03);  //Send pointer register address of Temperature
		//		WAIT;
		//		//this 00 00
		//		DATA(0x00);  //Send MSB of THigh
		//		WAIT;
		//
		//		DATA(0x00);  //Send LSB of THigh
		//		WAIT;
		//
		//		for(int i = 0; i < 10000; i++);
		//
		//		RESTART;
		//		DATA(TMP102_WRITE);   //Transmit first byte
		//		WAIT;
		//		DATA(0x03);  //Send pointer register address of Temperature
		//		WAIT;
		//		//this 00 00
		//		DATA(0x1C);  //Send MSB of THigh
		//		WAIT;
		//
		//		DATA(0x50);  //Send LSB of THigh
		//		WAIT;
		//	}


		RESTART;
		DATA(TMP102_WRITE);   //Transmit first byte
		WAIT;

		DATA(TMP102_TLOW);  //Send pointer register address of TLow
		WAIT;

		DATA(0x00);  //Send MSB of TLow
		WAIT;

		DATA(0x00);  //Send LSB of TLow
		WAIT;
		log_message(DEBUG, __func__, "TLOW registers initialised");
		log_message(TEST, __func__, "TLOW registers initialised");


		//Thigh
		RESTART;
		DATA(TMP102_WRITE);   //Transmit first byte
		WAIT;

		DATA(TMP102_THIGH);  //Send pointer register address of Temperature
		WAIT;

		DATA(0x00);  //Send MSB of THigh
		WAIT;

		DATA(0x00);  //Send LSB of THigh
		WAIT;
		log_message(DEBUG, __func__, "THIGH registers initialised");
		log_message(TEST, __func__, "THIGH registers initialised");
		//Configuration
		//		RESTART;
		//		DATA(TMP102_WRITE);   //Transmit first byte
		//		WAIT;
		//		DATA(0x01);  //Send pointer register address of config
		//		WAIT;
		//
		//		DATA(0x62);  //config interrupt mode
		//		WAIT;
		//
		//		DATA(0xA0);  //Send
		//		WAIT;

		setupOnce = true;
	}

}

uint16_t read_temp()
{
	uint8_t MSB = 0, LSB = 0;
	int32_t temp_read = 0;
	int16_t twos_complement;

	log_message(DEBUG, __func__, "Reading Temperature");
	log_message(TEST, __func__, "Reading Temperature");
	RESTART;
	DATA(TMP102_WRITE);   //Transmit first byte
	WAIT;
	DATA(TMP102_TEMPERATURE);  //Send pointer register address of Temperature
	WAIT;
	RESTART;

	DATA(TMP102_READ);  //Read Temperature
	WAIT;

	//read mode
	I2C1->C1 &= ~I2C_C1_TX_MASK;
	//NACK
	I2C1->C1 &= ~I2C_C1_TXAK_MASK;

	//dummy read
	I2C1->D;
	WAIT;

	MSB = I2C1->D;
	WAIT;

	STOP;
	LSB = I2C1->D;

	if( LSB == 0xFF )
		return 0xFFFF;  //Return error when it reads reserved value
	if((MSB & 0x80) == 0)
	{
		temp_read = (MSB << 4) | ((LSB >> 4) & 0x0F);  //Convert into 12 bit format
//		printf("%x\n", temp_read);
//		printf("The temperature is : %d\n", temp_read/16);
	}
	else
	{
		negative = true;
//		printf("Negative\n");
		twos_complement = (MSB << 4) | ((LSB >> 4) & 0x0F);  //Convert into 12 bit format
		temp_read = ((~twos_complement) & 0x0FF)  +1;  //Find twos complement
//		printf("%x\n", temp_read);
//		printf("The temperature is : -%d\n", (temp_read)/16);
	}

	if( (temp_read/16) < (-45) || (temp_read/16) > 125) //Check if temperature is out of range of sensor
	{
//		printf("Error in reading temp, function %s", __func__);
		log_message(DEBUG, __func__, "Error in reading temp");
		log_message(TEST, __func__, "Error in reading temp");
		log_message(NORMAL, __func__, "Error in reading temp");
		return 0xFFFF;
	}
	else
		return (temp_read/16);
}

