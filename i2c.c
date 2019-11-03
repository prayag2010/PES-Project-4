#include "../CMSIS/MKL25Z4.h"
#include <stdio.h>
void i2c_master_init()
{
	//uint8_t data;
	SIM->SCGC4 |= SIM_SCGC4_I2C1_MASK;  //Enabling the clock for I2C module
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	PORTE->PCR[0] |= PORT_PCR_MUX(6) | 0x03;   //SCL
	PORTE->PCR[1] |= PORT_PCR_MUX(6) | 0x03;   //SDA

	I2C1->F |= I2C_F_MULT(0) | I2C_F_ICR(0x11);   //Setting a baud rate of 300kbps
	I2C1->C1|= I2C_C1_IICEN_MASK; //Enabling I2C module

	I2C1->C1 |= I2C_C1_TX_MASK;  //Setting the master bit and enabling transmit mode
	I2C1->C1 |= I2C_C1_MST_MASK;
	//	I2C1->A1 |= I2C_A1_AD(0x48);   //Setting the slave address
	I2C1->D = I2C_D_DATA(0x90);   //Transmit first byte
	while(!(I2C1->S & I2C_S_IICIF_MASK)); //Wait to transmit first byte
	I2C1->S |= I2C_S_IICIF_MASK;

	I2C1->D = I2C_D_DATA(0x02);  //Send pointer register address of TLow
	while(!(I2C1->S & I2C_S_IICIF_MASK)); //Wait to transmit pointer address
	I2C1->S |= I2C_S_IICIF_MASK;

	I2C1->D = I2C_D_DATA(0x16);  //Send MSB of TLow
	while(!(I2C1->S & I2C_S_IICIF_MASK)); //Wait to transmit pointer address
	I2C1->S |= I2C_S_IICIF_MASK;

	I2C1->D = I2C_D_DATA(0x00);  //Send LSB of TLow
	while(!(I2C1->S & I2C_S_IICIF_MASK)); //Wait to transmit pointer address
	I2C1->S |= I2C_S_IICIF_MASK;
//	I2C1->C1 &= ~I2C_C1_TXAK_MASK;  //NACK
//	I2C1->C1 &= ~I2C_C1_MST_MASK;  //STOP

	I2C1->C1 |= I2C_C1_RSTA_MASK;  //RESTART
	I2C1->D = I2C_D_DATA(0x90);   //Transmit first byte
	while(!(I2C1->S & I2C_S_IICIF_MASK)); //Wait to transmit first byte
	I2C1->S |= I2C_S_IICIF_MASK;
	I2C1->D = I2C_D_DATA(0x02);  //Send pointer register address of TLow
	while(!(I2C1->S & I2C_S_IICIF_MASK)); //Wait to transmit pointer address
	I2C1->S |= I2C_S_IICIF_MASK;
	I2C1->C1 |= I2C_C1_RSTA_MASK;
	I2C1->D = I2C_D_DATA(0x90 | 0x01);  //Read TLOW
	while(!(I2C1->S & I2C_S_IICIF_MASK));
	I2C1->S |= I2C_S_IICIF_MASK;
	I2C1->C1 &= ~I2C_C1_TXAK_MASK;
	I2C1->C1 &= ~I2C_C1_TX_MASK;


	I2C1->D;
	while(!(I2C1->S & I2C_S_IICIF_MASK));
	I2C1->S |= I2C_S_IICIF_MASK;

	printf("TLOW Register contents\n");
	uint8_t MSB = I2C1->D;
	while(!(I2C1->S & I2C_S_IICIF_MASK));
	I2C1->S |= I2C_S_IICIF_MASK;
	printf("%x\n", MSB);

	uint8_t LSB = I2C1->D;
//	LSB = I2C1->D;
	while(!(I2C1->S & I2C_S_IICIF_MASK));
	I2C1->S |= I2C_S_IICIF_MASK;
	printf("%x\n", LSB);
//	I2C1->C1 &= ~I2C_C1_TXAK_MASK;  //NACK

//	if((!I2C1->S & I2C_S_RXAK_MASK))
//		printf("Slave found\n");
	I2C1->C1 |= I2C_C1_RSTA_MASK;  //RESTART
	I2C1->D = I2C_D_DATA(0x90);   //Transmit first byte
	while(!(I2C1->S & I2C_S_IICIF_MASK)); //Wait to transmit first byte
	I2C1->S |= I2C_S_IICIF_MASK;
	I2C1->D = I2C_D_DATA(0x00);  //Send pointer register address of TLow
	while(!(I2C1->S & I2C_S_IICIF_MASK)); //Wait to transmit pointer address
	I2C1->S |= I2C_S_IICIF_MASK;
//	I2C1->C1 |= I2C_C1_RSTA_MASK;
	I2C1->D = I2C_D_DATA(0x90 | 0x01);
}
