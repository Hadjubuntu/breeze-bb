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
	state = 0;
	b5 = 0;
	_iter = 0;
	altitudeMeters = 0.0f;
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
		printf("ac1=%hd\n", ac1);
		printf("ac2=%hd\n", ac2);
		printf("ac3=%hd\n", ac3);
		printf("ac4=%hd\n", ac4);
		printf("ac5=%hd\n", ac5);
		printf("ac6=%hd\n", ac6);
		printf("b1=%hd\n", b1);
		printf("b2=%hd\n", b2);
		printf("mb=%hd\n", mb);
		printf("mc=%hd\n", mc);
		printf("md=%hd\n", md);
	}
}

void Baro::process()
{
	if (baroHealthy) {
		if (state == 0)
		{
			state = 1;
			readUncompensatedTempValue();
		}

		// Force to check state again since it has changed above
		if (state == 3)
		{
			state = 5;
			readUncompensatedPressureValue();
		}

		// Again
		if (state == 6)
		{
			calculateTrueTemperature();

			calculateTruePressure();

			calculateAltitude();

			state = 0;

			// Note: limit iter variable to increase indefinitly
			if (_iter < 10000) {
				_iter ++;
			}
		}
	}
}

void Baro::callback()
{
	if (state == 1)
	{
		uint8_t Data[2];
		_i2c.readFrom(0xF6, 2, Data);
		uncompensatedTemperature = ((Data[0] << 8) | Data[1]);
		state = 3;

	}
	else if (state == 5)
	{
		uint8_t Data[3];
		_i2c.readFrom(0xF6, 3, Data);
		uncompensatedPressure = ((Data[0] << 16) | (Data[1] << 8) | Data[2]) >> (8 - OVERSAMPLING);
		state = 6;
	}
}

void Baro::readUncompensatedTempValue()
{
	_i2c.writeTo(0xF4, 0x2E);

	// Wait for 4.5ms
	planCallback(DateUtils::secondsToMicros(4.5f/1000.0f));
}


void Baro::readUncompensatedPressureValue()
{
	_i2c.writeTo(0xF4, 0x34 + (OVERSAMPLING << 6));
	// Wait for 7.5ms (oss=1) | 25.5ms (oss=3) //
	planCallback(DateUtils::secondsToMicros(26.0f/1000.0f));
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
	unsigned long b4, b7;
//	long p;


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
	truePressure  = b7 < 0x80000000 ? (b7 * 2) / b4 : (b7 / b4) * 2;

	x1 = (truePressure >> 8) * (truePressure >> 8);
	x1 = (x1 * 3038) >> 16;
	x2 = (-7357 * truePressure) >> 16;
	truePressure = truePressure + (truePressure + ((x1 + x2 + 3791) >> 4));
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
		altitudeMeters = 0.0f; // throw data
	}
	else if (_iter >= 20  && _iter <= 200)
	{
		float alpha = 0.1;
		// First value set to computed value
		if (firstMeasure) {
			alpha = 0.0;
			firstMeasure = false;
		}
		GroundPressure = (long) (alpha*GroundPressure + (1.0-alpha) * truePressure);
		GroundTemp =  (long) (alpha*GroundTemp  + (1.0-alpha) * trueTemperature);

		printf("Adjusting ground pressure : %lu | true pressure = %lu \n", GroundPressure, truePressure);

		altitudeMeters = 0.0;
	}
	else
	{
		float altitudeOffset = 0.1;

		// Calculate altitude from difference of pressure
		printf("true pressure = %.1f | ground pressure = %.1f | ground temp = %.1f\n", (float)truePressure, (float)GroundPressure, (float)GroundTemp/10.0);
		float diffPressure = ((float)truePressure / (float)GroundPressure);
		altitudeMeters =  altitudeOffset + 44330.0 * (1.0 - FastMath::fpow(diffPressure, 0.190295));
	}
}
