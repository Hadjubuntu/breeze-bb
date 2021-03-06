/*
 * Gyro.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: adrien
 */
#include <stdio.h>
#include "Gyro.h"


void Gyro::init()
{
	// Init
	provider.init();

	HAL::delayMs(250);

	int num_samples = 50;
	float accumulator[] = {0.0, 0.0, 0.0};

	// Throw first values (TODO improve code)
	for (int i = 0 ; i < num_samples ; i++)
	{
		update();
	}

	// Calibration
	for (int i = 0 ; i < num_samples ; i++)
	{
		update();

		accumulator[0] += gyroRaw.getX();
		accumulator[1] += gyroRaw.getY();
		accumulator[2] += gyroRaw.getZ();

		HAL::delayMs(5);
	}

	for(int i = 0 ; i < 3 ; i++)
	{
		accumulator[i] /= num_samples;
	}

	offset = accumulator;
}

void Gyro::update()
{
	// Read from provider
	Vect3D cGyro = provider.read();

	// Retrieve offset
	cGyro -= offset;

	// Store data and filter gyro output
	gyroRaw = cGyro ;

	// Filter gyro data
	gyroFiltered =  gyroFiltered * (1.0 - filterNewDataCoeff) + cGyro * filterNewDataCoeff;
}
