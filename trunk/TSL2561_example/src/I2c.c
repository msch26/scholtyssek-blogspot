/*
 * i2c.c
 *
 *  Copyright (C) 2014 Marco Scholtyssek <code@scholtyssek.org>
 *  Created on: 12.08.2014
 *      Author: scholtyssek
 */


#include "I2c.h"

void i2c_start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction) {
	// wait until I2C is not busy anymore
	while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
		;

	I2C_GenerateSTART(I2Cx, ENABLE);

	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
		;

	I2C_Send7bitAddress(I2Cx, address, direction);

	if (direction == I2C_Direction_Transmitter) {
		while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
			;
	} else if (direction == I2C_Direction_Receiver) {
		while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
			;
	}
}

/* This function transmits one byte to the slave
 * Parameters:
 *		I2Cx --> the I2C peripheral e.g. I2C1
 *		data --> the data byte to be transmitted
 */
void i2c_write(I2C_TypeDef* I2Cx, uint8_t data) {
	I2C_SendData(I2Cx, data);
	// wait until the byte has been transmitted
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		;
}

/* This function reads one byte from the slave device
 * and acknowledges the byte (requests another byte)
 */
uint8_t i2c_read_ack(I2C_TypeDef* I2Cx) {
	// enable acknowledge of recieved data
	I2C_AcknowledgeConfig(I2Cx, ENABLE);
	// wait until one byte has been received
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
		;
	// read data from I2C data register and return data byte
	uint8_t data = I2C_ReceiveData(I2Cx);
	return data;
}

/* This function reads one byte from the slave device
 * and doesn't acknowledge the received data
 * after that a STOP condition is transmitted
 */
uint8_t i2c_read_nack(I2C_TypeDef* I2Cx) {
	// disabe acknowledge of received data
	// nack also generates stop condition after last byte received
	// see reference manual for more info
	I2C_AcknowledgeConfig(I2Cx, DISABLE);
	I2C_GenerateSTOP(I2Cx, ENABLE);
	// wait until one byte has been received
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
		;
	// read data from I2C data register and return data byte
	uint8_t data = I2C_ReceiveData(I2Cx);
	return data;
}

/* This function issues a stop condition and therefore
 * releases the bus
 */
void i2c_stop(I2C_TypeDef* I2Cx) {
	// Send I2C1 STOP Condition
	I2C_GenerateSTOP(I2Cx, ENABLE);
	// wait for I2C1 EV8_2 --> byte has been transmitted
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		;
}

/*
 * Writes one byte to the given register address
 */
void i2c_send_byte(I2C_TypeDef* I2Cx, uint8_t registerAddress, uint8_t address, uint8_t byte){
	i2c_start(I2Cx, address, I2C_Direction_Transmitter);
	i2c_write(I2Cx, registerAddress);
	i2c_write(I2Cx, byte);
	i2c_stop(I2Cx);
}
