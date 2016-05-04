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
public:
	I2C();
	I2C(short DEV_ADD);
	I2C(short DEV_ADD, short busAddr);
	~I2C();

	int openConnection();

	void setBusAddress(short busAddr);
	short getBusAddress();

	void writeBit(short DATA_REGADD, short data, short bitNum);
	void writeMoreBits(short DATA_REGADD, short data, short length,
			short startBit);
	void writeTo(short DATA_REGADD, short data);
	void writeByteBuffer(short DATA_REGADD, short *data, short length);
	void writeByteArduino(int8_t data);
	void writeByteBufferArduino(short *data, short length);

	short readBit(short DATA_REGADD, short bitNum);
	short readMoreBits(short DATA_REGADD, short length, short startBit);
	short readByte(short DATA_REGADD);
	void readFrom(short DATA_REGADD,  short length, short *data);
	void readByteBufferArduino(short *data, short length);
	int16_t readWord(short MSB, short LSB);
	void setDEV_ADD(short DEV_ADD);

	/** Return instance */
	static I2C getInstance(short deviceAddr) {
		I2C instance(deviceAddr);
		return instance;
	}

private:
	short busAddr;
	short DEV_ADD;
	char *path;




};

#endif /* PERIPHERALS_I2C_I2C_H_ */
