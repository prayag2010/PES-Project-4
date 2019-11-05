/*
 * @file tmp
 * @brief Header file that contains macros
 *
 * This header file contains macros that are specific
 * to the I2C peripheral in the FRDM-MKL25Z board
 * that the registers in the TMP102 temperature
 * sensor
 *
 * @authors Rahul Ramaprasad, Prayag Milan Desai
 * @date November 1 2019
 * @verison 1.0
 */

#ifndef TMP_H_
#define TMP_H_

//Wait till no interrupt is pending
#define WAIT {while(!(I2C1->S & I2C_S_IICIF_MASK)); I2C1->S |= I2C_S_IICIF_MASK;}

//Setting the master bit to send START bit
#define START I2C1->C1 |= I2C_C1_MST_MASK;

//RESTART bit
#define RESTART I2C1->C1 |= I2C_C1_RSTA_MASK;

//Resetting master bit to send STOP bit
#define STOP I2C1->C1 &= ~I2C_C1_MST_MASK;

//Loading data into Data register
#define DATA(x) I2C1->D = I2C_D_DATA(x);

//Slave address in write mode
#define TMP102_WRITE 0x90

//Slave address in read mode
#define TMP102_READ 0x91

//Pointer address of TLOW register
#define TMP102_TLOW 0x02

//Pointer address of THIGH register
#define TMP102_THIGH 0x03

//Pointer address of TEMPERATURE register
#define TMP102_TEMPERATURE 0x00

#endif /* TMP_H_ */
