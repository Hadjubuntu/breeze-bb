/*
 * I2C.h
 *
 *  Created on: Sep 23, 2015
 *      Author: adrien
 */

#ifndef PERIPHERALS_I2C_I2C_H_
#define PERIPHERALS_I2C_I2C_H_
#include <linux/i2c-dev.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "HalTypes.h"

class I2C
{
protected:
	bool mock;
public:
	I2C();
	I2C(uint8_t DEV_ADD);
	I2C(uint8_t DEV_ADD, uint8_t busAddr);
	~I2C();

	int openConnection();

	void setBusAddress(uint8_t busAddr);
	uint8_t getBusAddress();

	void writeBit(uint8_t DATA_REGADD, uint8_t data, uint8_t bitNum);
	void writeMoreBits(uint8_t DATA_REGADD, uint8_t data, uint8_t length,
			uint8_t startBit);
	void writeTo(uint8_t DATA_REGADD, uint8_t data);
	void writeByteBuffer(uint8_t DATA_REGADD, uint8_t *data, uint8_t length);
	void writeByteArduino(int8_t data);
	void writeByteBufferArduino(uint8_t *data, uint8_t length);

	uint8_t readBit(uint8_t DATA_REGADD, uint8_t bitNum);
	uint8_t readMoreBits(uint8_t DATA_REGADD, uint8_t length, uint8_t startBit);
	uint8_t readByte(uint8_t DATA_REGADD);
	void readFrom(uint8_t DATA_REGADD,  uint8_t length, uint8_t *data);
	void readByteBufferArduino(uint8_t *data, uint8_t length);
	int16_t readWord(uint8_t MSB, uint8_t LSB);
	void setDEV_ADD(uint8_t DEV_ADD);

	/** Return instance */
	static I2C getInstance(uint8_t deviceAddr) {
		I2C instance(deviceAddr);
		return instance;
	}

private:
	uint8_t busAddr;
	uint8_t DEV_ADD;
	char *path;




};

#endif /* PERIPHERALS_I2C_I2C_H_ */
