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

int main(void)
{
	BOARD_InitDebugConsole();

	i2c_master_init();

	read_temp();

//	for (int i = 0; i < 900000; i++);
//	i2c_master_init();
//	read_temp();

//	__enable_irq();
//
//	NVIC->ICPR[0] |= 1 << I2C1_IRQn;
//    NVIC->ISER[0] |= 1 << I2C1_IRQn;


	return 0 ;
}

void I2C1_IRQHandler (void)
{
	if((I2C1->S & I2C_S_RXAK_MASK) == 0)           //Check if slave received a byte
	{
		printf("Slave detected\n");
		I2C1->S |= I2C_S_RXAK_MASK;
	}
}
