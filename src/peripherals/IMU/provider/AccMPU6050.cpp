/*
 * AccMPU6050.cpp
 *
 *  Created on: May 10, 2016
 *      Author: adrien
 */

#include "AccMPU6050.h"

////////Acceleration sensor ADXL345 function/////////////////////////////
#define ACC_ADR (0x68)    // Defined MPU6050 address, ALT ADDRESS pin is grounded
#define A_TO_READ (6) // Number of bytes to read(each axis accounted for two-byte)
// MPU6050 Acceleromter Control register
#define ACC_CONFIG    			 0x1C
#define ACC_MPU6050_BW_RATE      0x2C
#define ACC_MPU6050_POWER_CTL    0x2D
#define ACC_MPU6050_INT_ENABLE   0x2E
#define ACC_MPU6050_DATA_FORMAT  0x31
#define ACC_MPU6050_FIFO_CTL     0x38
#define ACC_MPU6050_DUR          0x21

//ADXL345 Data register
#define ACC_MPU6050_DEVID        0x00
#define ACC_MPU6050_DATAX0       0x32
#define ACC_MPU6050_DATAX1       0x33
#define ACC_MPU6050_DATAY0       0x34
#define ACC_MPU6050_DATAY1       0x35
#define ACC_MPU6050_DATAZ0       0x36
#define ACC_MPU6050_DATAZ1       0x37

#define ACC_LSB_TO_G 0.00048828125f

AccMPU6050::AccMPU6050() : IMUProvider(), i2c(I2C::getInstance(ACC_ADR))
{
	// Initialize i2c connection
}


void AccMPU6050::init()
{
	// Initialize accelerometer - WARNING gyro should be initialized first for power management
	// ---
	HAL::delayMs(10);
	i2c.writeTo(ACC_CONFIG, 3 << 3); // 16g
	HAL::delayMs(10);
}

int accStep = 0;
Vect3D AccMPU6050::read()
{
	uint8_t buffer[6];
	int16_t result[3];

	i2c.readFrom(0x3B, 6, buffer);

	// Positive right roll, pitching up, yaw right
	result[0]=(((int)buffer[0]) << 8 ) | buffer[1];
	result[1]=(((int)buffer[2]) << 8 ) | buffer[3];
	result[2]=-(((int)buffer[4]) << 8 ) | buffer[5];


	// Create accelerometer vector from raw data
	Vect3D cAcc(result[0], result[1], result[2]);

	// Scale gyro from LSB to g
	cAcc *= ACC_LSB_TO_G;

	if (accStep % 100 == 0)
	{
		printf("acc x=%.1f  y=%.1f  z=%.1f\n", cAcc.getX(), cAcc.getY(), cAcc.getZ());
	}
	accStep ++;


	return cAcc;
}


AccMPU6050::~AccMPU6050() {
	// TODO Auto-generated destructor stub
}
