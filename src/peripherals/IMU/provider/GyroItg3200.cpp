/*
 * GyroItg3200.cpp
 *
 *  Created on: May 10, 2016
 *      Author: adrien
 */

#include "GyroItg3200.h"


#define GYRO_ADDR 0x68
#define GYRO_PWR_MNGT_1 0x3e
#define GYRO_SMPLRT_DIV 0x15
#define GYRO_DLPF_FS 0x16
#define GYRO_INT_CFG 0x17
#define GYRO_REG_ADDR 0x1d
#define GYRO_LSB_TO_DEGS 0.06097560975f


GyroItg3200::GyroItg3200() : IMUProvider()
{
	i2c = I2C::getInstance(GYRO_ADDR);
}

void GyroItg3200::init()
{
	// Initialize gyro
	///----------------------------
	// Power control - reset and wake up
	i2c.writeTo(GYRO_PWR_MNGT_1,0x80);
	HAL::delayMs(100);
	i2c.writeTo(GYRO_PWR_MNGT_1,0x00);
	HAL::delayMs(5);
	// Sample at 500Hz (1kHz / (div + 1))
	i2c.writeTo(GYRO_SMPLRT_DIV, 0x01);
	HAL::delayMs(5);
	// Gyro +/- 2000 deg/s, low-pass filter at 10Hz
	i2c.writeTo(GYRO_DLPF_FS, 0x1d);

	// 1A => 0x00
	// 1B => 0x03 (2000°)

	HAL::delayMs(5);
	//
	i2c.writeTo(GYRO_INT_CFG, 0x00);
}


Vect3D GyroItg3200::read()
{
	// Read raw data
	short buff[6];

	i2c.readFrom(GYRO_REG_ADDR, 6, buff);

	// Convention : positive when rolling left, pitching down
	int result[3];
	result[1] = -((((int) buff[0]) << 8) | buff[1]) ;
	result[0] = -((((int) buff[2]) << 8) | buff[3]) ;
	result[2] = -((((int) buff[4]) << 8) | buff[5]) ;

	// Create gyro vector from raw data
	Vect3D cGyro(result[0], result[1], result[2]);

	// Scale gyro from LSB to deg/s
	cGyro *= GYRO_LSB_TO_DEGS;
	cGyro = cGyro.toRad();

	return cGyro;
}

GyroItg3200::~GyroItg3200() {
	// TODO Auto-generated destructor stub
}

