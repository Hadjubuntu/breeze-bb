/*
 * I2C.h
 *
 *  Created on: Sep 23, 2015
 *      Author: adrien
 */

#ifndef PERIPHERALS_I2C_I2C_H_
#define PERIPHERALS_I2C_I2C_H_

#include "../../hal/HalTypes.h"

class I2C {
private:
	uint8 _deviceAddr;

public:
	I2C(uint8 deviceAddr) : _deviceAddr(deviceAddr) {
//		reset();
	}

	/**
	 * Write to specific device at address
	 */
	int writeTo(uint8 addr, uint8 val);

	/**
	 * Read data from I2C address for a specific device
	 * @param addr Address
	 * @param nbBytes Number of bytes
	 * @param msgData output data
	 */
	int readFrom(uint8 addr, uint8 nbBytes, uint8 *msgData);


	void reset();


	/** Return instance */
	static I2C getInstance(uint8 deviceAddr) {
		I2C instance(deviceAddr);
		return instance;
	}

};

#endif /* PERIPHERALS_I2C_I2C_H_ */
