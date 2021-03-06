/*
 * SRF02.h
 *
 *  Created on: 12.08.2014
 *      Author: scholtyssek
 *
 */

#ifndef ULTRASONICSENSORCOMPONENT_H_
#define ULTRASONICSENSORCOMPONENT_H_
#include "I2c.h"
#include <stm32f4xx_gpio.h>

#define ULTRASONICSENSOR_I2C I2C3
#define ULTRASONICSENSOR_AF GPIO_AF_I2C3
#define ULTRASONICSENSOR_I2C_CLKSPEED 400000 // 40KHz
#define ULTRASONICSENSOR_RCC_I2C_CLOCK RCC_APB1Periph_I2C3
#define ULTRASONICSENSOR_SCL_PIN GPIO_Pin_8
#define ULTRASONICSENSOR_RCC_SCL_PORT_CLOCK RCC_AHB1Periph_GPIOA
#define ULTRASONICSENSOR_SCL_PORT GPIOA

#define ULTRASONICSENSOR_SCL_PINSOURCE GPIO_PinSource8
#define ULTRASONICSENSOR_SDA_PIN GPIO_Pin_9
#define ULTRASONICSENSOR_SDA_PORT GPIOC
#define ULTRASONICSENSOR_RCC_SDA_PORT_CLOCK RCC_AHB1Periph_GPIOC
#define ULTRASONICSENSOR_SDA_PINSOURCE GPIO_PinSource9

#define SENSOR1_ADDRESS 0xE0
#define SENSOR2_ADDRESS 0xE2
#define SENSOR3_ADDRESS 0xE4
#define SENSOR4_ADDRESS 0xE6

#define COMMAND_REGISTER 0x00

void initSRF02(void);
uint16_t readAutotuneDistance(uint8_t sensorAddress);
uint16_t readDistance(uint8_t sensorAddress);
void setSensorI2CAddress(uint8_t address, uint8_t newAddress);
#endif
