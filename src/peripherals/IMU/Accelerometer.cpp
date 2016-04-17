/*
 * Accelerometer.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: adrien
 */
#include <stdio.h>
#include "../../hal/HAL.h"
#include "Accelerometer.h"

/**
 * Reference:
 * https://github.com/opendrone/flymaple/blob/testing/src/Accelerometer.cpp
 */
void Accelerometer::init()
{
	uint8 buff[1];
	_i2c.readFrom(0x00, 1, buff);

	// now we check msg_data for our 0xE5 magic number
	uint8 dev_id = buff[0];


	if (dev_id != XL345_DEVID)
	{
		// TODO exit
	}

	//invoke ADXL345
	_i2c.writeTo(ADXLREG_POWER_CTL,0x00);	HAL::delayMs(5);	//
	_i2c.writeTo(ADXLREG_POWER_CTL, 0xff);	HAL::delayMs(5);	//
	_i2c.writeTo(ADXLREG_POWER_CTL, 0x08); HAL::delayMs(5);	//
	_i2c.writeTo(ADXLREG_DATA_FORMAT, 0x08); HAL::delayMs(5);	//
	_i2c.writeTo(ADXLREG_BW_RATE, 0x09); HAL::delayMs(5);		//25Hz


	HAL::delayMs(100);

	// Calibrate: compute accelerometer scale and offset
	//	calibration();

	// DEBUG 0 offset
	_offset = Vect3D(0.0, 0.0, 0.0);
}

void Accelerometer::calibration()
{
	// Collect samples, throw strange sample (when moving too much)
	int nbHealthySamples = 100;
	int nbTrial = 0;
	int countSample = 0;
	float sumAccX = 0.0;
	float sumAccY = 0.0;
	float sumAccZ = 0.0;
	float maxAccZ = 0.0;

	update();
	Vect3D prevAccRaw = _accRaw;

	while (countSample < nbHealthySamples && nbTrial < 1000)
	{
		// Collect data
		update();

		// Sample vector
		Vect3D delta = prevAccRaw - _accRaw;



		if (delta.getNorm2() < 0.05)
		{
			sumAccX += _accRaw.getX();
			sumAccY += _accRaw.getY();
			sumAccZ += _accRaw.getZ();
			//
			if ((_accRaw.getZ() > 0.0 && _accRaw.getZ() > maxAccZ)
					|| (_accRaw.getZ() < 0.0 && _accRaw.getZ() < maxAccZ)) {
				maxAccZ = _accRaw.getZ();
			}

			prevAccRaw = _accRaw;
			countSample ++;
		}

		HAL::delayMs(5);
	}

	if (nbTrial >= 1000 && countSample < nbHealthySamples) {
		// No calibration could be done..
		_offset = Vect3D(0.0, 0.0, 0.0);
	}
	else
	{
		_offset = Vect3D(sumAccX / countSample, sumAccY / countSample,  0.0); // sumAccZ / countSample - maxAccZ
	}
}

void Accelerometer::update()
{
	// Retrieve raw data from I2C
	uint8 buff[A_TO_READ];

	_i2c.readFrom(ADXLREG_DATAX0, A_TO_READ, buff);

	int16 result[3];
	result[1] = -((((int16) buff[1]) << 8) | buff[0]) ;
	result[0] = -((((int16) buff[3]) << 8) | buff[2]) ;
	result[2] = -((((int16) buff[5]) << 8) | buff[4]) ;

	// Create vector 3D from array of int16
	Vect3D cAcc = Vect3D::fromInt16Array(result);

	// Apply scale factor from LSB to g
	cAcc *= ACC_SENSITIVITY;

	// Retrieve offset
	cAcc -= _offset;

	_accRaw = cAcc;

	//	_accFiltered = cAcc;
	_accFiltered = _accFiltered * (1.0 - _filterNewDataCoeff) + cAcc * _filterNewDataCoeff;

}
