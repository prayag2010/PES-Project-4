	/*
 * @file i2c.h
 * @brief Header file for controlling the I2C registers
 *
 * This header file has prototypes for functions that
 * turns I2C on and connects to the temperature sensor.
 *
 * @authors Rahul Ramaprasad, Prayag Milan Desai
 * @date November 1 2019
 * @verison 1.0
 */

#ifndef I2C_H_
#define I2C_H_

#include "../CMSIS/MKL25Z4.h"
#include "tmp.h"
#include <stdbool.h>

extern bool negative;
extern bool postCheck;

/*
 * @brief Initializes the I2C registers
 *
 * Enables the I2C module and sends the address
 * to the slave and sets Tlow
 *
 * @return void
 */
void i2c_master_init();


/*
 * @brief Reads the Temperature
 *
 * Reads the temperature from the
 * temperature register in the sensor
 * and returns the temperature in
 * Celsius or 0xFFFF in case of any error
 *
 * @return uint16_t
 */
uint16_t read_temp();


/*
 * @brief Send start bit
 *
 * Start the i2c transaction
 *
 * @return void
 */
void start();


#endif /* I2C_H_ */
