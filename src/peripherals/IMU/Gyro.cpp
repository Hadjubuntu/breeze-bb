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
	provider->init();

	HAL::delayMs(250);

	int num_samples = 50;
	float accumulator[] = {0.0, 0.0, 0.0};

	for(int i = 0 ; i < num_samples ; i++)
	{
		update();

		accumulator[0] += _gyroRaw.getX();
		accumulator[1] += _gyroRaw.getY();
		accumulator[2] += _gyroRaw.getZ();

		HAL::delayMs(5);
	}

	for(int i = 0 ; i < 3 ; i++)
	{
		accumulator[i] /= num_samples;
	}

	_offset = accumulator;
}

void Gyro::update()
{
	// Read from provider
	Vect3D cGyro = provider->read();

	// Retrieve offset
	cGyro -= _offset;

	// Store data and filter gyro output
	_gyroRaw = cGyro ;

	// Filter gyro data
	_gyroFiltered =  _gyroFiltered * (1.0 - _filterNewDataCoeff) + cGyro * _filterNewDataCoeff;
}
