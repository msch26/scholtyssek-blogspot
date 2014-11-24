/*
 * Configuration.h
 *
 *  Copyright (C) 2014 Marco Scholtyssek <code@scholtyssek.org>
 *  Created on: 07.07.2014
 *      Author: scholtyssek
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * 	 	TSL2561 light sensor		* * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define LIGHTSENSOR_I2C I2C1
#define LIGHTSENSOR_AF GPIO_AF_I2C1
#define LIGHTSENSOR_I2C_CLKSPEED 400000 // 400 kHz
#define LIGHTSENSOR_RCC_I2C_CLOCK RCC_APB1Periph_I2C1

#define LIGHTSENSOR_RCC_PORT_CLOCK RCC_AHB1Periph_GPIOB
#define LIGHTSENSOR_SCL_PIN GPIO_Pin_6
#define LIGHTSENSOR_SCL_PINSOURCE GPIO_PinSource6
#define LIGHTSENSOR_SDA_PIN GPIO_Pin_7
#define LIGHTSENSOR_SDA_PINSOURCE GPIO_PinSource7
#define LIGHTSENSOR_PORT GPIOB

#endif /* CONFIGURATION_H_ */
