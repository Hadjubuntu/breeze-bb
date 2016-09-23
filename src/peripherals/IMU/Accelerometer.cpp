/*
 * Accelerometer.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: adrien
 */
#include <stdio.h>
#include "../../hal/HAL.h"
#include "../../math/common/FastMath.h"
#include "Accelerometer.h"


void Accelerometer::init()
{
	// Init
	provider.init();

	HAL::delayMs(250);

	int num_samples = 50;
	float accumulator[] = {0.0, 0.0, 0.0};

	for(int i = 0 ; i < num_samples ; i++)
	{
		update();

		accumulator[0] += accRaw.getX();
		accumulator[1] += accRaw.getY();
		accumulator[2] += accRaw.getZ();

		HAL::delayMs(5);
	}

	for(int i = 0 ; i < 3 ; i++)
	{
		accumulator[i] /= num_samples;
	}

//	offset = accumulator;

	calibration();
}

void Accelerometer::update()
{
	// Read from provider
	Vect3D cAcc = provider.read();


	// Retrieve offset
		cAcc -= offset;

	accRaw = cAcc;

	//	_accFiltered = cAcc;
	accFiltered = accFiltered * (1.0 - filterNewDataCoeff) + cAcc * filterNewDataCoeff;


	printf("gx=%.1f; gy=%.1f; gz=%.1f\n",
			FastMath::toDegrees(accFiltered.getX()), FastMath::toDegrees(accFiltered.getY()), FastMath::toDegrees(accFiltered.getZ()));
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
	Vect3D prevAccRaw = accRaw;

	while (countSample < nbHealthySamples && nbTrial < 1000)
	{
		// Collect data
		update();

		// Sample vector
		Vect3D delta = prevAccRaw - accRaw;


		if (delta.getNorm2() < 0.05)
		{
			sumAccX += accRaw.getX();
			sumAccY += accRaw.getY();
			sumAccZ += accRaw.getZ();
			//
			if ((accRaw.getZ() > 0.0 && accRaw.getZ() > maxAccZ)
					|| (accRaw.getZ() < 0.0 && accRaw.getZ() < maxAccZ)) {
				maxAccZ = accRaw.getZ();
			}

			prevAccRaw = accRaw;
			countSample ++;
		}

		HAL::delayMs(5);
	}

	if (nbTrial >= 1000 && countSample < nbHealthySamples) {
		// No calibration could be done..
		offset = Vect3D(0.0, 0.0, 0.0);
	}
	else
	{
		offset = Vect3D(sumAccX / countSample, sumAccY / countSample,  0.0); // sumAccZ / countSample - maxAccZ
	}
}
