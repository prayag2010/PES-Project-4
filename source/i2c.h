/*
 * i2c.h
 *
 *  Created on: Oct 30, 2019
 *      Author: rahul
 */

#ifndef I2C_H_
#define I2C_H_

#include "../CMSIS/MKL25Z4.h"
#include "tmp.h"

void i2c_master_init();

uint16_t read_temp();

void start();


#endif /* I2C_H_ */
