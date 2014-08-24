/*
 * SRF02.c
 *
 *  Created on: 12.08.2014
 *      Author: scholtyssek
 */

#include "SRF02.h"

uint8_t readRegister(uint8_t sensorAddress, uint8_t _register);


uint8_t receivedRanging_data[2];
uint16_t rangingData[4];		/* Sensoren 1 - 4 */

uint16_t readDistance(uint8_t sensorAddress);

/*
 * The sensors are connected to I2C3 (PA8 and PC9)
 */
void initSRF02(){
	I2C_DeInit(ULTRASONICSENSOR_I2C);
	uint16_t data = 0;

	GPIO_InitTypeDef GPIO_InitStruct;
	I2C_InitTypeDef I2C_InitStruct;

	RCC_APB1PeriphClockCmd(ULTRASONICSENSOR_RCC_I2C_CLOCK, ENABLE);
	RCC_AHB1PeriphClockCmd(ULTRASONICSENSOR_RCC_SCL_PORT_CLOCK | ULTRASONICSENSOR_RCC_SDA_PORT_CLOCK, ENABLE);

	GPIO_InitStruct.GPIO_Pin =  ULTRASONICSENSOR_SDA_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(ULTRASONICSENSOR_SDA_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = ULTRASONICSENSOR_SCL_PIN;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(ULTRASONICSENSOR_SCL_PORT, &GPIO_InitStruct);
	// Connect I2C1 pins to AF
	GPIO_PinAFConfig(ULTRASONICSENSOR_SCL_PORT, ULTRASONICSENSOR_SCL_PINSOURCE, ULTRASONICSENSOR_AF);	// SCL
	GPIO_PinAFConfig(ULTRASONICSENSOR_SDA_PORT, ULTRASONICSENSOR_SDA_PINSOURCE, ULTRASONICSENSOR_AF); 	// SDA

	I2C_InitStruct.I2C_ClockSpeed = ULTRASONICSENSOR_I2C_CLKSPEED; 		// 40kHz frequency for the sensors
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Disable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

	I2C_Init(ULTRASONICSENSOR_I2C, &I2C_InitStruct);

	I2C_Cmd(ULTRASONICSENSOR_I2C, ENABLE);

	uint8_t d = 0;
	uint16_t minimumRange = 0;

	initUltrasonicSensorI2C(SENSOR1_ADDRESS);
	initUltrasonicSensorI2C(SENSOR2_ADDRESS);
	initUltrasonicSensorI2C(SENSOR3_ADDRESS);
	initUltrasonicSensorI2C(SENSOR4_ADDRESS);
}

/*
 * Konfiguration einer dauerhaften I2C Adresse fuer einen Sensor
 *
 * Sets a persistent address I2C address to the sensor
 */
void setSensorI2CAddress(uint8_t address, uint8_t newAddress){
	i2cSendByte(ULTRASONICSENSOR_I2C, address, COMMAND_REGISTER, 0xA0);
	i2cSendByte(ULTRASONICSENSOR_I2C, address, COMMAND_REGISTER, 0xAA);
	i2cSendByte(ULTRASONICSENSOR_I2C, address, COMMAND_REGISTER, 0xA5);
	i2cSendByte(ULTRASONICSENSOR_I2C, address, COMMAND_REGISTER, newAddress);
}

void initUltrasonicSensorI2C(uint8_t address) {
	I2C_start(ULTRASONICSENSOR_I2C, address, I2C_Direction_Transmitter); // start in Master transmitter mode
	I2C_write(ULTRASONICSENSOR_I2C, 0x00); 								 // command register address
	I2C_write(ULTRASONICSENSOR_I2C, 0x51); 								 // measuring unit in [cm]
	I2C_stop(ULTRASONICSENSOR_I2C); 									 // stop the transmission
}

uint8_t readRegister(uint8_t sensorAddress, uint8_t _register){
		I2C_start(ULTRASONICSENSOR_I2C, sensorAddress, I2C_Direction_Transmitter);
		I2C_write(ULTRASONICSENSOR_I2C, _register);
		I2C_stop(ULTRASONICSENSOR_I2C);
		I2C_start(ULTRASONICSENSOR_I2C, sensorAddress, I2C_Direction_Receiver);
		return I2C_read_nack(ULTRASONICSENSOR_I2C);
}

uint16_t readDistance(uint8_t sensorAddress){
	receivedRanging_data[0] = readRegister(SENSOR1_ADDRESS, 0x03);	// low byte
	receivedRanging_data[1] = readRegister(SENSOR1_ADDRESS, 0x02);	// high byte
	return (receivedRanging_data[1] << 8) | receivedRanging_data[0];
}

uint16_t readAutotuneDistance(uint8_t sensorAddress){
	uint16_t minimumRange = readRegister(sensorAddress, 0x04);		// autotune minimum low byte
	minimumRange |= ((readRegister(sensorAddress, 0x05)) << 1);		// autotune minimum high byte
	return minimumRange;
}
