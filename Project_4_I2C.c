/**
 * @file    Project_4_I2C.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include <stdbool.h>

#include "i2c.h"

int mainmkk(void)
{
	BOARD_InitDebugConsole();
	i2c_master_init();

	//__enable_irq();

	//NVIC->ICPR[0] |= 1 << I2C1_IRQn;
    //NVIC->ISER[0] |= 1 << I2C1_IRQn;

	while(!(I2C1->S & I2C_S_IICIF_MASK));
	I2C1->S |= I2C_S_IICIF_MASK;
	I2C1->C1 &= ~I2C_C1_TXAK_MASK;
	I2C1->C1 &= ~I2C_C1_TX_MASK;


	I2C1->D;
	while(!(I2C1->S & I2C_S_IICIF_MASK));
	I2C1->S |= I2C_S_IICIF_MASK;

//	printf("Temperature Register contents\n");
	uint8_t MSB = I2C1->D;
	while(!(I2C1->S & I2C_S_IICIF_MASK));
	I2C1->S |= I2C_S_IICIF_MASK;
//	printf("%x\n", MSB);

	uint8_t LSB = I2C1->D;
	while(!(I2C1->S & I2C_S_IICIF_MASK));
	I2C1->S |= I2C_S_IICIF_MASK;
//	printf("%x\n", LSB);

	if((MSB & 0x80) == 0)
	{
		uint16_t temp_read = (MSB << 4) | ((LSB >> 4) & 0x0F);
		printf("%x\n", temp_read);
		//double temp_value = (double)temp_read;
		//printf("%f\n", temp_value);
		printf("The temperature is : %d\n", temp_read/16);
	}
	else
	{
		uint16_t temp_read = (MSB << 4) | ((LSB >> 4) & 0x0F);
		printf("%x\n", temp_read);
		uint16_t twos_complement = ((~temp_read) & 0x0FF)  +1;
		printf("%x\n", twos_complement);
		printf("The temperature is : -%d\n", (twos_complement)/16);
	}
	return 0 ;
}

void I2C1_IRQHandler (void)
{
	if((I2C1->S & I2C_S_RXAK_MASK) == 0)           //Check if slave received a byte
	{
		printf("Slave detected\n");
		I2C1->S |= I2C_S_RXAK_MASK;
	}
	//		if(I2C1->S & I2C_S_TCF_MASK)            //Check if master transmitted a byte
	//			printf("Sent Data from Master \n");
	//		I2C1->S |= ~I2C_S_TCF_MASK;
	//		I2C1->S |= ~I2C_S_IAAS_MASK;
	//
	//		if(I2C1->S & (~I2C_S_BUSY_MASK))
	//			printf("Bus is not busy now\n");
}
