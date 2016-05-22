/*
 * GyroMPU6050.cpp
 *
 *  Created on: May 10, 2016
 *      Author: adrien
 */

#include "GyroMPU6050.h"

#define GYRO_ADDR 0x68
#define GYRO_PWR_MNGT_1 0x6B
#define GYRO_PWR_MNGT_2 0x6C
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
#define INT_PIN_CFG 0x37
#define GYRO_SMPLRT_DIV 0x15
#define GYRO_DLPF_FS 0x16
#define GYRO_REG_ADDR 0x43
#define GYRO_SAMPLE_RATE 0x19
#define GYRO_LSB_TO_DEGS 0.06097560975f


GyroMPU6050::GyroMPU6050() : GyroProvider(), i2c(I2C::getInstance(GYRO_ADDR))
{

}

void GyroMPU6050::init()
{
	// Initialize gyro
	///----------------------------
//	 Power control - reset and wake up
	i2c.writeTo(GYRO_PWR_MNGT_1,0x80);
	HAL::delayMs(100);
	i2c.writeTo(GYRO_PWR_MNGT_1,0x00);
	HAL::delayMs(10);
//	i2c.writeTo(GYRO_PWR_MNGT_2,0x00); // Enable acc & gyro
	HAL::delayMs(10);
	i2c.writeTo(CONFIG, 0x01); // Use DLPF set gyro bandwith 184Hz
	HAL::delayMs(10);
	i2c.writeTo(GYRO_CONFIG, 0x18); // +/- 2000 dps
	HAL::delayMs(10);
	i2c.writeTo(GYRO_SAMPLE_RATE, 1000 / 800 - 1); // 800 Hz
	HAL::delayMs(10);


	// Sample at 500Hz (1kHz / (div + 1))
	//	i2c.writeTo(GYRO_SMPLRT_DIV, 0x01);
	HAL::delayMs(5);
	// Gyro +/- 2000 deg/s, low-pass filter at 10Hz
	//	i2c.writeTo(GYRO_DLPF_FS, 0x1d);

	HAL::delayMs(5);

}


Vect3D GyroMPU6050::read()
{
	short buffer[6];
	int16_t result[3];

	i2c.readFrom(0x43, 6, buffer);

	result[0]=(((int16_t)buffer[0]) << 8 ) | buffer[1];
	result[1]=(((int16_t)buffer[2]) << 8 ) | buffer[3];
	result[2]=(((int16_t)buffer[4]) << 8 ) | buffer[5];

	result[0] *= 2000.0/32768.0;
	result[1] *= 2000.0/32768.0;
	result[2] *= 2000.0/32768.0;

	printf("%d; %d; %d\n", result[0], result[1], result[2]);

	//	// Read raw data
	//	short buff[6];
	//
	//	i2c.readFrom(GYRO_REG_ADDR, 6, buff);
	//
	//	// Convention : positive when rolling left, pitching down
	//	int result[3];
	//	result[1] = -((((int) buff[0]) << 8) | buff[1]) ;
	//	result[0] = -((((int) buff[2]) << 8) | buff[3]) ;
	//	result[2] = -((((int) buff[4]) << 8) | buff[5]) ;

	// Create gyro vector from raw data
	Vect3D cGyro(result[0], result[1], result[2]);

	// Scale gyro from LSB to deg/s
	cGyro *= GYRO_LSB_TO_DEGS;
	cGyro = cGyro.toRad();

	return cGyro;
}

GyroMPU6050::~GyroMPU6050() {
	// TODO Auto-generated destructor stub
}

