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
	short _deviceAddr;

public:
	I2C(short deviceAddr) : _deviceAddr(deviceAddr) {
//		reset();
	}

	/**
	 * Write to specific device at address
	 */
	int writeTo(short addr, short val);

	/**
	 * Read data from I2C address for a specific device
	 * @param addr Address
	 * @param nbBytes Number of bytes
	 * @param msgData output data
	 */
	int readFrom(short addr, short nbBytes, short *msgData);


	void reset();


	/** Return instance */
	static I2C getInstance(short deviceAddr) {
		I2C instance(deviceAddr);
		return instance;
	}

};

#endif /* PERIPHERALS_I2C_I2C_H_ */
