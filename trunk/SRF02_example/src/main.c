/*
 * main.c
 *
 *  Created on: 24.08.2014
 *      Author: scholtyssek
 *
 *  In this example there are four SRF02 sensors connected to the STM32F4 via I2C bus
 */

#include "SRF02.h"

uint16_t distance[4];
uint16_t minimumRange[4];

int main(int argc, char **argv) {
	uint8_t address = 0x00;
	initSRF02();

	/* Sets the I2C address */
//	while(1){
//		initUltrasonicSensorI2C(SENSOR1_ADDRESS);
//		setSensorI2CAddress(SENSOR1_ADDRESS, 0xF0);
//	}

	/* Receive sensor data */
	while (1) {
		for (int i = 0; i < 4; i++) {
			switch (i) {
			case 1:
				address = SENSOR2_ADDRESS;
				break;
			case 2:
				address = SENSOR3_ADDRESS;
				break;
			case 3:
				address = SENSOR4_ADDRESS;
				break;
			default:
				address = SENSOR1_ADDRESS;
				break;
			}

			initUltrasonicSensorI2C(address);

			distance[i] = readDistance(address);
		}
	}
}
