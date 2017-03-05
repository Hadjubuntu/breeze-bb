/**
 * breeze-arm
 * Sonar.cpp
 * ----------------------------
 *
 * ----------------------------
 *  Created on: Jan 22, 2016
 *      Author: Adrien HADJ-SALAH
 */

#include "Sonar.h"
#include "../../../math/common/VectMath.h"
#include "../../../math/common/FastMath.h"
//
//int nSonar = 5;
//History<float> sonarValues(nSonar);

Sonar::Sonar() : Processing()
{
	freqHz = 10;
	sonarPin = 10;
	filteredSonarValueCm = 0.0;
	healthy = false;
}

void Sonar::init()
{
	//	pinMode(_sonarPin, INPUT);

	// Create zero vector at initialization
	//	for (int i = 0; i < nSonar; i ++)
	//	{
	//		sonarValues.add(0.0);
	//	}

//	int strangeMeasure = 0;
//	int nbMeasure = 25;
//	for (int i = 0; i < 25; i ++) {
//		//		float currentSonarVal = (float) analogRead(_sonarPin) * 0.3175;
//		float currentSonarVal = 0.0;
//
//		if (currentSonarVal > 150) {
//			strangeMeasure ++;
//		}
//	}
//
//	if (((float)strangeMeasure / nbMeasure) > 0.7) {
//		_healthy = false;
//		//		Serial3.println("Sonar is not healthy");
//	}
}

void Sonar::process()
{
	if (healthy)
	{
		// Read new value
		float currentSonarVal = 0.0; // FIXME (float) analogRead(_sonarPin) * 0.3175; //

		if (filteredSonarValueCm == 0.0)
		{
			filteredSonarValueCm = currentSonarVal;
		}
		else {
			float delta = abs(currentSonarVal - filteredSonarValueCm);
			float sign = currentSonarVal > filteredSonarValueCm ? 1.0 : -1.0;

			float k = 1.0/(delta/8.0+1.0);
			filteredSonarValueCm = filteredSonarValueCm + k * sign * delta;
		}

		//		// Filter measured value by using mean delta
		//		float deltaMean = VectMath::derivate(sonarValues.toVector());
		//		float deltaMeasured = currentSonarVal - sonarValues.getLast();
		//
		//		float filteredValue = currentSonarVal;
		//
		//		// Do not consider spike value due to analog wrong reads
		//		if (FastMath::fabs(deltaMeasured) >= 150) {
		//			filteredValue = sonarValues.getLast();
		//		}
		//		else if (FastMath::fabs(deltaMeasured) > FastMath::fabs(deltaMean)) {
		//			filteredValue = sonarValues.getLast() + deltaMeasured / 10.0;
		//		}
		//		// Store it to history
		//		sonarValues.add(filteredValue);
		//		//
		//		//	// Apply least-square filter
		//		_filteredSonarValueCm = _filter.apply(sonarValues.toVector(), nSonar + 1);
	}
}

