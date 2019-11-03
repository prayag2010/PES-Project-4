#include "../CMSIS/MKL25Z4.h"
#include <stdio.h>
#include "i2c.h"

void i2c_master_init()
{

	SIM->SCGC4 |= SIM_SCGC4_I2C1_MASK;  //Enabling the clock for I2C module
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	PORTE->PCR[0] |= PORT_PCR_MUX(6) | 0x03;   //SCL
	PORTE->PCR[1] |= PORT_PCR_MUX(6) | 0x03;   //SDA

	I2C1->F |= I2C_F_MULT(0) | I2C_F_ICR(0x11);   //Setting a baud rate of 300kbps
	I2C1->C1|= I2C_C1_IICEN_MASK; //Enabling I2C module

	I2C1->C1 |= I2C_C1_TX_MASK;  //Enabling transmit mode
	START;
	DATA(0x90);  //Transmit first byte
	WAIT;

	DATA(0x02);  //Send pointer register address of TLow
	WAIT;

	DATA(0x16);  //Send MSB of TLow
	WAIT;

	DATA(0x00);  //Send LSB of TLow
	WAIT;
}

uint16_t read_temp()
{
	uint8_t MSB = 0, LSB = 0;
	uint16_t temp_read = 0;

	RESTART;
	DATA(0x90);   //Transmit first byte
	WAIT;
	DATA(0x00);  //Send pointer register address of Temperature
	WAIT;
	RESTART;

	DATA(0x91);  //Read Temperature
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

	if((MSB & 0x80) == 0)
	{
		temp_read = (MSB << 4) | ((LSB >> 4) & 0x0F);
		printf("%x\n", temp_read);
		printf("The temperature is : %d\n", temp_read/16);
		printf("%x\n",MSB);
		printf("%x\n",LSB);
	}
	else
	{
		uint16_t twos_complement = (MSB << 4) | ((LSB >> 4) & 0x0F);
		printf("%x\n", twos_complement);
		temp_read = ((~twos_complement) & 0x0FF)  +1;
		printf("%x\n", temp_read);
		printf("The temperature is : -%d\n", (temp_read)/16);
	}
	return temp_read;
}
