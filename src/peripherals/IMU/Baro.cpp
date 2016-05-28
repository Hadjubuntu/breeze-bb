/*
 * Baro.cpp

 *
 *  Created on: Oct 11, 2015
 *      Author: adrien
 */
#include <stdio.h>
#include "../../hal/HAL.h"
#include "../../math/common/FastMath.h"
#include "Baro.h"

Baro::Baro() : Processing(), _i2c(I2C::getInstance(BMP085_ADDRESS))
{
	freqHz = 20;

	dev_address = BMP085_ADDRESS;
	pressure_samples = 1;
	retry_time = 0;
	trueTemperature = 0.0;
	uncompensatedTemperature = 0;
	uncompensatedPressure = 0;
	ac1 = 0; ac2= 0; ac3 = 0; ac4 = 0; ac5 = 0;
	ac6 = 0; b1 = 0; b2 = 0; mb = 0;
	truePressure = 0;
	mc = 0;
	firstMeasure = true;
	GroundPressure = 1;
	GroundTemp = 0;
	last_update = 0; md = 0;
	_count = 0;
	_state = 0;
	b5 = 0;
	_iter = 0;
	_altitudeMeters = 0.0f;
	baroHealthy = true;
}

void Baro::init()
{
	uint8_t buff[22];


	// We read the calibration data registers
	_i2c.readFrom(0xAA, 22, buff) ;

	ac1 = ((int16_t)buff[0] << 8) | buff[1];
	ac2 = ((int16_t)buff[2] << 8) | buff[3];
	ac3 = ((int16_t)buff[4] << 8) | buff[5];
	ac4 = ((int16_t)buff[6] << 8) | buff[7];
	ac5 = ((int16_t)buff[8] << 8) | buff[9];
	ac6 = ((int16_t)buff[10] << 8) | buff[11];
	b1 = ((int16_t)buff[12] << 8) | buff[13];
	b2 = ((int16_t)buff[14] << 8) | buff[15];
	mb = ((int16_t)buff[16] << 8) | buff[17];
	mc = ((int16_t)buff[18] << 8) | buff[19];
	md = ((int16_t)buff[20] << 8) | buff[21];

	if (ac1 == 0) {
		baroHealthy = false;
		printf("ERROR: Barometer can't be initialized\n");
	}
	else {
		printf("Barometer initialized successfully\n");
	}
}

void Baro::process()
{
	if (baroHealthy) {
		if (_state == 0)
		{
			_state = 1;
			readUncompensatedTempValue();
		}

		// Force to check state again since it has changed above
		if (_state == 3)
		{
			_state = 5;
			readUncompensatedPressureValue();
		}

		// Again
		if (_state == 6)
		{
			calculateTrueTemperature();

			calculateTruePressure();

			calculateAltitude();

			_state = 0;

			// Note: limit iter variable to increase indefinitly
			if (_iter < 10000) {
				_iter ++;
			}
		}
	}
}

void Baro::callback()
{
	if (_state == 1)
	{
		uint8_t Data[2];
		_i2c.readFrom(0xF6, 2, Data);
		uncompensatedTemperature = ((Data[0] << 8) | Data[1]);
		_state = 3;

	}
	else if (_state == 5)
	{
		uint8_t Data[3];
		_i2c.readFrom(0xF6, 3, Data);
		uncompensatedPressure = ((Data[0] << 16) | (Data[1] << 8) | Data[2]) >> (8 - OVERSAMPLING);
		_state = 6;
	}
}

void Baro::readUncompensatedTempValue()
{
	_i2c.writeTo(0xF4, 0x2E);

	// Wait for 4.5ms
	planCallback(DateUtils::secondsToMicros(4.5/1000.0));
}


void Baro::readUncompensatedPressureValue()
{
	_i2c.writeTo(0xF4, 0x34 + (OVERSAMPLING << 6));
	// Wait for 7.5ms (oss=1) | 25.5ms (oss=3)
	planCallback(DateUtils::secondsToMicros(25.6f/1000.0));
}

void Baro::calculateTrueTemperature()
{
	long x1 = (uncompensatedTemperature - ac6) * ac5 >> 15;
	long x2 = ((long) mc << 11) / (x1 + md);
	b5 = x1 + x2;
	trueTemperature = (b5 + 8) >> 4;
}

void Baro::calculateTruePressure()
{
	long b6, b3,  x1, x2, x3;
	long tmpPressure;
	unsigned long b4, b7;
	unsigned long tmp;
	long p;


	b6 = b5 - 4000;
	x1 = (b2 * (b6 * b6 >> 12)) >> 11;
	x2 = ac2 * b6 >> 11;
	x3 = x1 + x2;
	b3 = ((((int32_t)ac1 * 4 + x3) << OVERSAMPLING) + 2) >> 2;
	x1 = ac3 * b6 >> 13;
	x2 = (b1 * (b6 * b6 >> 12)) >> 16;
	x3 = ((x1+x2) + 2) >> 2;
	b4 = (ac4 * (uint32_t)(x3 + 32768)) >> 15;
	b7 = ((uint32_t)uncompensatedPressure - b3) * (50000 >> OVERSAMPLING);
	p  = b7 < 0x80000000 ? (b7 * 2) / b4 : (b7 / b4) * 2;

	x1 = (p >> 8) * (p >> 8);
	x1 = (x1 * 3038) >> 16;
	x2 = (-7357 * p) >> 16;
	truePressure = 0.8*truePressure + 0.2*(p + ((x1 + x2 + 3791) >> 4));
}

void Baro::recalibrateAtZeroThrottle()
{
	float alpha = 0.05;
	GroundPressure = (long) (alpha*GroundPressure + (1.0-alpha) * truePressure);
	GroundTemp =  (long) (alpha*GroundTemp  + (1.0-alpha) * trueTemperature);
}

void Baro::calculateAltitude()
{
	// Calibration value
	if (_iter < 20) {
		_altitudeMeters = 0.0f; // throw data
	}
	else if (_iter >= 20  && _iter <= 100)
	{
		float alpha = 0.7;
		// First value set to computed value
		if (firstMeasure) {
			alpha = 0.0;
			firstMeasure = false;
		}
		GroundPressure = (long) (alpha*GroundPressure + (1.0-alpha) * truePressure);
		GroundTemp =  (long) (alpha*GroundTemp  + (1.0-alpha) * trueTemperature);

		_altitudeMeters = 0.0;
	}
	else
	{
		float altitudeOffset = 0.1;

		// Calculate altitude from difference of pressure
		printf("true pressure = %.1f | ground pressure = %.1f\n", (float)truePressure, (float)GroundPressure);
		float diffPressure = ((float)truePressure / (float)GroundPressure);
		_altitudeMeters =  altitudeOffset + 44330.0 * (1.0 - FastMath::fpow(diffPressure, 0.190295));
	}
}
