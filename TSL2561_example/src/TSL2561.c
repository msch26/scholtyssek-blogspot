/*
 * LightSensor.c
 *
 *  Copyright (C) 2014 Marco Scholtyssek <code@scholtyssek.org>
 *  Created on: 14.07.2014
 *      Author: scholtyssek
 */

#include "TSL2561.h"

uint8_t received_data[2];
uint16_t data = 0;

void initLightSensorI2C(void);
uint16_t readAdcChannel(uint8_t lowByteAddress, uint8_t highByteAddress);

/*
 * The lightsensor is connected to I2C1 (PB6 and PB7)
 */
void init_lightsensor_component() {
	I2C_DeInit(LIGHTSENSOR_I2C);
	uint16_t data = 0;

	GPIO_InitTypeDef GPIO_InitStruct;
	I2C_InitTypeDef I2C_InitStruct;

	// enable APB1 peripheral clock for I2C1
	RCC_APB1PeriphClockCmd(LIGHTSENSOR_RCC_I2C_CLOCK, ENABLE);
	// enable clock for SCL and SDA pins
	RCC_AHB1PeriphClockCmd(LIGHTSENSOR_RCC_PORT_CLOCK, ENABLE);

	/*
	 * 1. SCL on PB6 and SDA on PB7
	 * 2. SCL on PB8 and SDA on PB9
	 */
	GPIO_InitStruct.GPIO_Pin = LIGHTSENSOR_SCL_PIN | LIGHTSENSOR_SDA_PIN; 	// we are going to use PB6 and PB7
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; 													// set pins to alternate function
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;											// set GPIO speed
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(LIGHTSENSOR_PORT, &GPIO_InitStruct);												// init the port

	// Connect I2C1 pins to AF
	GPIO_PinAFConfig(LIGHTSENSOR_PORT, LIGHTSENSOR_SCL_PINSOURCE, LIGHTSENSOR_AF);	// SCL
	GPIO_PinAFConfig(LIGHTSENSOR_PORT, LIGHTSENSOR_SDA_PINSOURCE, LIGHTSENSOR_AF); 	// SDA

	// configure I2C
	I2C_InitStruct.I2C_ClockSpeed = LIGHTSENSOR_I2C_CLKSPEED; 						// 400kHz bus frequency
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Disable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

	I2C_Init(LIGHTSENSOR_I2C, &I2C_InitStruct);

	// enable I2C1
	I2C_Cmd(LIGHTSENSOR_I2C, ENABLE);

	initLightSensorI2C();
}

/*
 * Init function for the light sensor. the sensor has to be activated by 0x3 (see p. 14)
 * As integration time 402ms will be used (p.15)
 */
void initLightSensorI2C(void) {
	/* ENABLE DEVICE to set the configuration */
	i2c_start(LIGHTSENSOR_I2C, SLAVE_ADDRESS << 1, I2C_Direction_Transmitter); 		// start a transmission in Master transmitter mode
	i2c_write(LIGHTSENSOR_I2C, 0xA0); 																			// 10010000 - // Command Register (0x00)  CommandBit (0x80), Word protocol
	i2c_write(LIGHTSENSOR_I2C, 0x03); 																			// Power up the device (0x3)
	i2c_stop(LIGHTSENSOR_I2C); 																					// stop the transmission

	i2c_start(LIGHTSENSOR_I2C, SLAVE_ADDRESS << 1, I2C_Direction_Receiver); 			// start a transmission in Master transmitter mode
	received_data[0] = i2c_read_ack(LIGHTSENSOR_I2C); 												// 0x03 returns if the controller is active
	received_data[1] = i2c_read_nack(LIGHTSENSOR_I2C); 											// read one byte and request another byte
}

/*
 * There are the two seperate channels on the sensor, that detect the light.
 * Ch1 detects only the infrarated light spectrum and ch0 detects the visible and the infrarated light spectrum.
 * To calibrate the sensor to the human seeable light spectrum, ch1 will be subtracted from ch0.
 * */
unsigned long read_intensity() {
	long ch0 = readAdcChannel(CHANNEL0_LOW_BYTE_ADDRESS,
			CHANNEL0_HIGH_BYTE_ADDRESS);
	long ch1 = readAdcChannel(CHANNEL1_LOW_BYTE_ADDRESS,
			CHANNEL1_HIGH_BYTE_ADDRESS);
	unsigned long ratio1 = 0;

	data = 0;

	// calculate ratio and scale to 512 (2^9)
	if (ch0 != 0) {
		ratio1 = (ch1 << (RATIO_SCALE + 1)) / ch0;
	}

	// round ratio value
	unsigned long ratio = (ratio1 + 1) >> 1;

	/* transform to LUX (p. 23) */
	// FN package
	unsigned int b, m;

	if (ratio >= 0 && ratio <= K1T) {
		b = B1T;
		m = M1T;
	} else if (ratio <= K2T) {
		b = B2T;
		m = M2T;
	} else if (ratio <= K3T) {
		b = B3T;
		m = M3T;
	} else if (ratio <= K4T) {
		b = B4T;
		m = M4T;
	} else if (ratio <= K5T) {
		b = B5T;
		m = M5T;
	} else if (ratio <= K6T) {
		b = B6T;
		m = M6T;
	} else if (ratio <= K7T) {
		b = B7T;
		m = M7T;
	} else if (ratio <= K8T) {
		b = B8T;
		m = M8T;
	}

	unsigned long temp = (ch0 * b) - (ch1 * m);

	// avoid negativ LUX values
	if(temp < 0){
		temp = 0;
	}

	// round the LSB (2^LUX-SCALE-1)
	temp += (1 << LUX_SCALE-1);

	// cut the rest
	unsigned long lux = temp >> LUX_SCALE;

	return lux;
}

uint16_t readAdcChannel(uint8_t lowByteAddress, uint8_t highByteAddress) {
	received_data[0] = 0;
	received_data[1] = 0;

	i2c_start(LIGHTSENSOR_I2C, SLAVE_ADDRESS << 1, I2C_Direction_Transmitter);			// start a transmission in Master transmitter mode

	i2c_write(LIGHTSENSOR_I2C, lowByteAddress);

	i2c_stop(LIGHTSENSOR_I2C); 																						// stop the transmission

	i2c_start(LIGHTSENSOR_I2C, SLAVE_ADDRESS << 1, I2C_Direction_Receiver); 				// start a transmission in Master receiver mode
	received_data[0] = i2c_read_nack(LIGHTSENSOR_I2C); // read data0 low byte

	i2c_start(LIGHTSENSOR_I2C, SLAVE_ADDRESS << 1, I2C_Direction_Transmitter); 			// start a transmission in Master transmitter mode
	i2c_write(LIGHTSENSOR_I2C, highByteAddress);
	i2c_stop(LIGHTSENSOR_I2C); 																						// stop the transmission

	for (uint16_t i = 0; i < 65535; i++) {
	}

	i2c_start(LIGHTSENSOR_I2C, SLAVE_ADDRESS << 1, I2C_Direction_Receiver); 				// start a transmission in Master receiver mode
	received_data[1] = i2c_read_nack(LIGHTSENSOR_I2C); 												// read data0 high byte

	data = (uint16_t) received_data[0];
	data |= (uint16_t) (received_data[1] << 8);

	return data;
}
