/**
 * breeze-arm
 * Sonar.h
 * ----------------------------
 *
 * ----------------------------
 *  Created on: Jan 22, 2016
 *      Author: Adrien HADJ-SALAH
 */

#ifndef PROCESSING_NAV_SONAR_SONAR_H_
#define PROCESSING_NAV_SONAR_SONAR_H_

#include "../../../core/Processing.h"
#include "../../../core/History.h"
#include "../../../math/filter/LeastSquareFilter.h"

class Sonar : public Processing {
private:
	int _sonarPin;
	bool _healthy;
	float _filteredSonarValueCm;
	LeastSquareFilter _filter;
public:
	Sonar();

	void init();

	void process();
	void callback() { };

	float getOutput() {
		return _filteredSonarValueCm;
	}

	bool isHealthy() {
		return _healthy;
	}

};

#endif /* PROCESSING_NAV_SONAR_SONAR_H_ */
