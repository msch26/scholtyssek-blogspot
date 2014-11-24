/*
 * LightSensorComponent.h
 *
 *  Copyright (C) 2014 Marco Scholtyssek <code@scholtyssek.org>
 *  Created on: 14.07.2014
 *      Author: scholtyssek
 */

#ifndef LIGHTSENSORCOMPONENT_H_
#define LIGHTSENSORCOMPONENT_H_

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "I2c.h"
#include "Configuration.h"

#define SLAVE_ADDRESS 0x39 							// slave Address can also be 0x29, 0x39, 0x49
#define CHANNEL0_LOW_BYTE_ADDRESS 0x8C	// 1000 1100 - CommandBit and Address of CH0 Data lower Byte Register. Read data low of channel0 Register 0xC with byte protocol
#define CHANNEL0_HIGH_BYTE_ADDRESS 0x8D	// 1000 1101 - CommandBit and Address of CH0 Data upper Byte Register. Read data high of channel0 Register 0xD with byte protocol
#define CHANNEL1_LOW_BYTE_ADDRESS 0x8E
#define CHANNEL1_HIGH_BYTE_ADDRESS 0x8F

/*
 * Constants for the LUX calculation for the FN package (mechanic structure of the sensor)
 *
 * */
#define LUX_SCALE 14	// 2^14
#define RATIO_SCALE 9	// scale ratio to 2^5=512
#define K1T 0x0040 		// 0.125 * 2^RATIO_SCALE
#define B1T 0x01f2 		// 0.0304 * 2^LUX_SCALE
#define M1T 0x01be 		// 0.0272 * 2^LUX_SCALE
#define K2T 0x0080 		// 0.250 * 2^RATIO_SCALE
#define B2T 0x0214 		// 0.0325 * 2^LUX_SCALE
#define M2T 0x02d1		// 0.0440 * 2^LUX_SCALE
#define K3T 0x00c0		// 0.375 * 2^RATIO_SCALE
#define B3T 0x023f 		// 0.0351 * 2^LUX_SCALE
#define M3T 0x037b 		// 0.0544 * 2^LUX_SCALE
#define K4T 0x0100 		// 0.50 * 2^RATIO_SCALE
#define B4T 0x0270 		// 0.0381 * 2^LUX_SCALE
#define M4T 0x03fe 		// 0.0624 * 2^LUX_SCALE
#define K5T 0x0138 		// 0.61 * 2^RATIO_SCALE
#define B5T 0x016f 		// 0.0224 * 2^LUX_SCALE
#define M5T 0x01fc 		// 0.0310 * 2^LUX_SCALE
#define K6T 0x019a 		// 0.80 * 2^RATIO_SCALE
#define B6T 0x00d2 		// 0.0128 * 2^LUX_SCALE
#define M6T 0x00fb 		// 0.0153 * 2^LUX_SCALE
#define K7T 0x029a 		// 1.3 * 2^RATIO_SCALE
#define B7T 0x0018 		// 0.00146 * 2^LUX_SCALE
#define M7T 0x0012 		// 0.00112 * 2^LUX_SCALE
#define K8T 0x029a 		// 1.3 * 2^RATIO_SCALE
#define B8T 0x0000 		// 0.000 * 2^LUX_SCALE
#define M8T 0x0000 		// 0.000 * 2^LUX_SCALE



void init_lightsensor(void);

/*
 * returns the calculated light intensity in LUX
 */
unsigned long read_intensity(void);

#endif /* LIGHTSENSORCOMPONENT_H_ */
