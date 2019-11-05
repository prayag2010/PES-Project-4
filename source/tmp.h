/*
 * tmp.h
 *
 *  Created on: 04-Nov-2019
 *      Author: prayag
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




#endif /* TMP_H_ */
