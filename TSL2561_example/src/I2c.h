/*
 * i2c.h
 *
 *  Copyright (C) 2014 Marco Scholtyssek <code@scholtyssek.org>
 *  Created on: 12.08.2014
 *      Author: scholtyssek
 */

#ifndef I2C_H_
#define I2C_H_

#include "stm32f4xx_i2c.h"

uint8_t i2c_read_nack(I2C_TypeDef* I2Cx);
void i2c_stop(I2C_TypeDef* I2Cx);
uint8_t i2c_read_ack(I2C_TypeDef* I2Cx);
void i2c_write(I2C_TypeDef* I2Cx, uint8_t data);
void i2c_start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction);
void i2c_send_byte(I2C_TypeDef* I2Cx, uint8_t registerAddress, uint8_t address, uint8_t byte);
#endif /* I2C_H_ */
