/*
 * I2C.cpp
 *
 *  Created on: Sep 23, 2015
 *      Author: adrien
 */


#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "I2C.h"

/**
 * Read https://datko.net/2013/11/03/bbb_i2c/
 * to understand i2c bus
 */
#define I2C_BUS 2
#define PATH_SIZE 15

/** Default Constructor
 * @funtion BBB_I2C()
 *
 */
I2C::I2C() {
	mock = false;
	DEV_ADD = 0;
	busAddr = I2C_BUS;
	path = (char*) calloc(PATH_SIZE, sizeof(char));
	sprintf(path, "/dev/i2c-%d", this->busAddr);

	printf("Device address auto-set 0.");
	printf("I2C bus auto-set 1.");
}

/**
 * @function BBB_I2C(short DEV_ADD)
 * @param DEV_ADD Device Address
 */
I2C::I2C(short pDevAddr) {
	mock = false;
	busAddr = I2C_BUS;
	path = (char*) calloc(PATH_SIZE, sizeof(char));
	DEV_ADD = pDevAddr;
	sprintf(path, "/dev/i2c-%d", busAddr);

	printf("I2C bus auto-set 1.");
}

/**
 * @funtion BBB_I2C(short DEV_ADD, short busAddr)
 * @param DEV_ADD Device Address
 * @param busAddr I2C Bus address.
 */
I2C::I2C(short DEV_ADD, short busAddr) {
	mock = false;
	this->busAddr = busAddr;
	this->path = (char*) calloc(PATH_SIZE, sizeof(char));
	this->DEV_ADD = DEV_ADD;
	sprintf(path, "/dev/i2c-%d", this->busAddr);
}

/** Default Destructor
 * @funtion ~BBB_I2C()
 *
 */
I2C::~I2C() {
	free(path);
}

/**
 * @funtion setBusAddress(short busAddr)
 * @param busAddr I2C Bus address.
 */
void I2C::setBusAddress(short busAddr) {
	free(path);
	this->busAddr = busAddr;
	this->path = (char*) calloc(PATH_SIZE, sizeof(char));
	sprintf(path, "/dev/i2c-%d", this->busAddr);
}

/**
 * @funtion setDEV_ADD(short DEV_ADD)
 * @param DEV_ADD Device Address
 */
void I2C::setDEV_ADD(short DEV_ADD) {
	this->DEV_ADD = DEV_ADD;
}

/**
 * @function openConnection()
 * @return file type of int
 */
int I2C::openConnection() {
	int file;

	if ((file = open(path, O_RDWR)) < 0) {
		printf("%s do not open. Address %d.", path, DEV_ADD);
		mock = true;
	}

	if (ioctl(file, I2C_SLAVE, DEV_ADD) < 0) {
		printf("Can not join I2C Bus. Address %d.", DEV_ADD);
		mock = true;
	}

	return file;
}

/**
 * @function writeBit(short DEV_ADD, short DATA_REGADD, short data, int bitNum)
 * @param DEV_ADD Device Address.
 * @param DATA_REGADD Data Register Address.
 * @param data Writing data.
 * @param bitNum Bit Number for writing.
 * @return void.
 */
void I2C::writeBit(short DATA_REGADD, short data, short bitNum) {
	int8_t temp = readByte(DATA_REGADD);
	if (data == 0) {
		temp = temp & ~(1 << bitNum);
	} else if (data == 1) {
		temp = temp | (1 << bitNum);
	} else {
		printf("Value must be 0 or 1! --> Address %d.", DEV_ADD);
	}

	writeTo(DATA_REGADD, temp);

}

/**
 * @function writeBits(short DEV_ADD, short DATA_REGADD, short data, int length, int startBit)
 * @param DEV_ADD Device Address.
 * @param DATA_REGADD Data Register Address.
 * @param length Bits length.
 * @param startBit Starting point of the data.
 * @return void.
 */
void I2C::writeMoreBits(short DATA_REGADD, short data, short length,
		short startBit) {
	int8_t temp = readByte(DATA_REGADD);
	short bits = 1;
	short i = 0;

	while (i < length - 1) {
		bits = (bits << 1);
		++bits;
		++i;
	}

	temp &= ~(bits << startBit);

	temp |= (data << startBit);

	writeTo(DATA_REGADD, temp);

}

/**
 * @function writeByte(short DEV_ADD, short DATA_REGADD, short data)
 * @param DEV_ADD Device Address.
 * @param DATA_REGADD Data Register Address.
 * @param data Writing data.
 * @return void.
 */
void I2C::writeTo(short DATA_REGADD, short data) {

	int file = openConnection();

	if (!mock) {
		short buffer[2];

		buffer[0] = DATA_REGADD;
		buffer[1] = data;

		if (write(file, buffer, 2) != 2) {
			printf("Can not write data. Address %d.", DEV_ADD);
		}

		close(file);
	}
}

/**
 * @function writeByteBuffer(short DEV_ADD, short DATA_REGADD, short *data, short length)
 * @param DEV_ADD Device Address.
 * @param DATA_REGADD Data Register Address.
 * @param *data Data storage array.
 * @param length Array length.
 * @return void.
 */
void I2C::writeByteBuffer(short DATA_REGADD, short *data,
		short length) {

	int file = openConnection();

	short buffer[1];
	buffer[0] = DATA_REGADD;

	if (write(file, buffer, 1) != 1) {
		printf("Can not write data. Address %d.", DEV_ADD);
	}

	if (write(file, data, length) != length) {
		printf("Can not write data. Address %d.", DEV_ADD);
	}

	close(file);
}

/**
 * @function writeByteArduino(short DEV_ADD, int8_t data)
 * @param DEV_ADD Arduino Device Address.
 * @param data Writing data.
 * @return void.
 */
void I2C::writeByteArduino(int8_t data) {

	int file = openConnection();

	int8_t buffer[1];
	buffer[0] = data;

	if (write(file, buffer, 1) != 1) {
		printf("Can not write data. Address %d.", DEV_ADD);
	}

	close(file);

}

/**
 * @function writeByteBufferArduino(short DEV_ADD, short *data, short length)
 * @param DEV_ADD Arduino Device Address.
 * @param *data Data storage array.
 * @param length Array length.
 * @return void.
 */
void I2C::writeByteBufferArduino(short *data, short length) {

	int file = openConnection();

	if (write(file, data, length) != length) {
		printf("Can not write data. Address %d.", DEV_ADD);
	}

	close(file);
}

/**
 * @function readBit(short DEV_ADD, short DATA_REGADD, short bitNum)
 * @param DEV_ADD Device Address.
 * @param DATA_REGADD Data Register Address.
 * @param bitNum Bit Number for reading.
 * @return short bit value.
 */

short I2C::readBit(short DATA_REGADD, short bitNum) {
	int8_t temp = readByte(DATA_REGADD);
	return (temp >> bitNum) % 2;
}

/**
 * @function readBits(short DEV_ADD, short DATA_REGADD, short length, short startBit)
 * @param DEV_ADD Device Address.
 * @param DATA_REGADD Data Register Address.
 * @param length Bits length.
 * @param startBit Starting point of the value.
 * @return short bit value.
 */
short I2C::readMoreBits(short DATA_REGADD, short length,
		short startBit) {
	int8_t temp = readByte(DATA_REGADD);
	return (temp >> startBit) % (uint8_t) pow(2, length);
}

/**
 * @function readByte(short DEV_ADD, short DATA_REGADD)
 * @param DEV_ADD Device Address.
 * @param DATA_REGADD Data Register Address.
 * @return short bit value.
 */
short I2C::readByte(short DATA_REGADD) {

	int file = openConnection();

	short buffer[1];
	buffer[0] = DATA_REGADD;

	if (write(file, buffer, 1) != 1) {
		printf("Can not write data. Address %d.", DEV_ADD);
	}

	short value[1];

	if (read(file, value, 1) != 1) {
		printf("Can not read data. Address %d.", DEV_ADD);
	}

	close(file);

	return value[0];
}

/**
 * @function readByteBuffer(short DEV_ADD, short DATA_REGADD, short *data, short length)
 * @param DEV_ADD Device Address.
 * @param DATA_REGADD Data Register Address.
 * @param *data Data storage array.
 * @param length Array length.
 * @return void.
 */
void I2C::readFrom(short DATA_REGADD,
		short length,  short *data) {

	int file = openConnection();

	if (mock) {
		for (int i = 0; i < length; i ++)
		{
			data[i] = 0;
		}
	}
	else {

		short buffer[1];
		buffer[0] = DATA_REGADD;

		if (write(file, buffer, 1) != 1) {
			printf("Can not write data. Address %d.", DEV_ADD);
		}

		if (read(file, data, length) != length) {
			printf("Can not read data. Address %d.", DEV_ADD);
		}

		close(file);
	}

}

/**
 * @function readByteBufferArduino(short DEV_ADD, uint8_t* data, short length)
 * @param DEV_ADD Arduino Device Address.
 * @param *data Data storage array.
 * @param length Array length.
 * @return void.
 */
void I2C::readByteBufferArduino(short* data, short length) {

	int file = openConnection();

	if (read(file, data, length) != length) {
		printf("Can not read data. Address %d.", DEV_ADD);
	}

	close(file);

}

/**
 * @function readWord(short DEV_ADD, short MSB, short LSB)
 * @param DEV_ADD Arduino Device Address.
 * @param MSB 16-bit values Most Significant Byte Address.
 * @param LSB 16-bit values Less Significant Byte Address..
 * @return void.
 */
int16_t I2C::readWord(short MSB, short LSB) {

	short msb = readByte(MSB);

	short lsb = readByte(LSB);

	return ((int16_t) msb << 8) + lsb;
}
