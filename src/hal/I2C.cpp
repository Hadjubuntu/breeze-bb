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
#define I2C_BUS 1
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
 * @function BBB_I2C(uint8_t DEV_ADD)
 * @param DEV_ADD Device Address
 */
I2C::I2C(uint8_t pDevAddr) {
	mock = false;
	busAddr = I2C_BUS;
	path = (char*) calloc(PATH_SIZE, sizeof(char));
	DEV_ADD = pDevAddr;
	sprintf(path, "/dev/i2c-%d", busAddr);

	printf("I2C bus auto-set 1.");
}

/**
 * @funtion BBB_I2C(uint8_t DEV_ADD, uint8_t busAddr)
 * @param DEV_ADD Device Address
 * @param busAddr I2C Bus address.
 */
I2C::I2C(uint8_t DEV_ADD, uint8_t busAddr) {
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
 * @funtion setBusAddress(uint8_t busAddr)
 * @param busAddr I2C Bus address.
 */
void I2C::setBusAddress(uint8_t busAddr) {
	free(path);
	this->busAddr = busAddr;
	this->path = (char*) calloc(PATH_SIZE, sizeof(char));
	sprintf(path, "/dev/i2c-%d", this->busAddr);
}

/**
 * @funtion setDEV_ADD(uint8_t DEV_ADD)
 * @param DEV_ADD Device Address
 */
void I2C::setDEV_ADD(uint8_t DEV_ADD) {
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
 * @function writeBit(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t data, int bitNum)
 * @param DEV_ADD Device Address.
 * @param DATA_REGADD Data Register Address.
 * @param data Writing data.
 * @param bitNum Bit Number for writing.
 * @return void.
 */
void I2C::writeBit(uint8_t DATA_REGADD, uint8_t data, uint8_t bitNum) {
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
 * @function writeBits(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t data, int length, int startBit)
 * @param DEV_ADD Device Address.
 * @param DATA_REGADD Data Register Address.
 * @param length Bits length.
 * @param startBit Starting point of the data.
 * @return void.
 */
void I2C::writeMoreBits(uint8_t DATA_REGADD, uint8_t data, uint8_t length,
		uint8_t startBit) {
	int8_t temp = readByte(DATA_REGADD);
	uint8_t bits = 1;
	uint8_t i = 0;

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
 * @function writeByte(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t data)
 * @param DEV_ADD Device Address.
 * @param DATA_REGADD Data Register Address.
 * @param data Writing data.
 * @return void.
 */
void I2C::writeTo(uint8_t DATA_REGADD, uint8_t data) {

	int file = openConnection();

	if (!mock) {
		uint8_t buffer[2];

		buffer[0] = DATA_REGADD;
		buffer[1] = data;

		int res = write(file, buffer, 2);
		if (res != 2) {
			printf("Can not write data. Address %d. Error %d.\n", DEV_ADD, res);
		}

		close(file);
	}
}

/**
 * @function writeByteBuffer(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t *data, uint8_t length)
 * @param DEV_ADD Device Address.
 * @param DATA_REGADD Data Register Address.
 * @param *data Data storage array.
 * @param length Array length.
 * @return void.
 */
void I2C::writeByteBuffer(uint8_t DATA_REGADD, uint8_t *data,
		uint8_t length) {

	int file = openConnection();

	uint8_t buffer[1];
	buffer[0] = DATA_REGADD;

	if (write(file, buffer, 1) != 1) {
		printf("Can not write data. Address %d.\n", DEV_ADD);
	}

	if (write(file, data, length) != length) {
		printf("Can not write data. Address %d.\n", DEV_ADD);
	}

	close(file);
}

/**
 * @function writeByteArduino(uint8_t DEV_ADD, int8_t data)
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
 * @function writeByteBufferArduino(uint8_t DEV_ADD, uint8_t *data, uint8_t length)
 * @param DEV_ADD Arduino Device Address.
 * @param *data Data storage array.
 * @param length Array length.
 * @return void.
 */
void I2C::writeByteBufferArduino(uint8_t *data, uint8_t length) {

	int file = openConnection();

	if (write(file, data, length) != length) {
		printf("Can not write data. Address %d.", DEV_ADD);
	}

	close(file);
}

/**
 * @function readBit(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t bitNum)
 * @param DEV_ADD Device Address.
 * @param DATA_REGADD Data Register Address.
 * @param bitNum Bit Number for reading.
 * @return uint8_t bit value.
 */

uint8_t I2C::readBit(uint8_t DATA_REGADD, uint8_t bitNum) {
	int8_t temp = readByte(DATA_REGADD);
	return (temp >> bitNum) % 2;
}

/**
 * @function readBits(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t length, uint8_t startBit)
 * @param DEV_ADD Device Address.
 * @param DATA_REGADD Data Register Address.
 * @param length Bits length.
 * @param startBit Starting point of the value.
 * @return uint8_t bit value.
 */
uint8_t I2C::readMoreBits(uint8_t DATA_REGADD, uint8_t length,
		uint8_t startBit) {
	int8_t temp = readByte(DATA_REGADD);
	return (temp >> startBit) % (uint8_t) pow(2, length);
}

/**
 * @function readByte(uint8_t DEV_ADD, uint8_t DATA_REGADD)
 * @param DEV_ADD Device Address.
 * @param DATA_REGADD Data Register Address.
 * @return uint8_t bit value.
 */
uint8_t I2C::readByte(uint8_t DATA_REGADD) {

	int file = openConnection();

	uint8_t buffer[1];
	buffer[0] = DATA_REGADD;

	int res = write(file, buffer, 1);
	if (res != 1) {
		printf("Can not write data. Address %d. Error %d.\n", DEV_ADD, res);
	}

	uint8_t value[1];

	if (read(file, value, 1) != 1) {
		printf("Can not read data. Address %d.\n", DEV_ADD);
	}

	close(file);

	return value[0];
}

/**
 * @function readByteBuffer(uint8_t DEV_ADD, uint8_t DATA_REGADD, uint8_t *data, uint8_t length)
 * @param DEV_ADD Device Address.
 * @param DATA_REGADD Data Register Address.
 * @param *data Data storage array.
 * @param length Array length.
 * @return void.
 */
void I2C::readFrom(uint8_t DATA_REGADD,
		uint8_t length,  uint8_t *data) {

	int file = openConnection();

	if (mock) {
		for (int i = 0; i < length; i ++)
		{
			data[i] = 0;
		}
	}
	else {

		uint8_t buffer[1];
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
 * @function readByteBufferArduino(uint8_t DEV_ADD, uint8_t* data, uint8_t length)
 * @param DEV_ADD Arduino Device Address.
 * @param *data Data storage array.
 * @param length Array length.
 * @return void.
 */
void I2C::readByteBufferArduino(uint8_t* data, uint8_t length) {

	int file = openConnection();

	if (read(file, data, length) != length) {
		printf("Can not read data. Address %d.", DEV_ADD);
	}

	close(file);

}

/**
 * @function readWord(uint8_t DEV_ADD, uint8_t MSB, uint8_t LSB)
 * @param DEV_ADD Arduino Device Address.
 * @param MSB 16-bit values Most Significant Byte Address.
 * @param LSB 16-bit values Less Significant Byte Address..
 * @return void.
 */
int16_t I2C::readWord(uint8_t MSB, uint8_t LSB) {

	uint8_t msb = readByte(MSB);

	uint8_t lsb = readByte(LSB);

	return ((int16_t) msb << 8) + lsb;
}
