#ifndef I2C_H_
#define I2C_H_


#include "stm32f4xx_i2c.h"

uint8_t I2C_read_nack(I2C_TypeDef* I2Cx);
void I2C_stop(I2C_TypeDef* I2Cx);
uint8_t I2C_read_ack(I2C_TypeDef* I2Cx);
void I2C_write(I2C_TypeDef* I2Cx, uint8_t data);
void I2C_start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction);
void i2cSendByte(I2C_TypeDef* I2Cx, uint8_t address, uint8_t byte);

#endif /* I2C_H_ */
