/*
 * FlightControl.h
 *
 *  Created on: Oct 21, 2015
 *      Author: adrien
 */

#ifndef PROCESSING_FLIGHTSTABILIZATION_FLIGHTCONTROL_H_
#define PROCESSING_FLIGHTSTABILIZATION_FLIGHTCONTROL_H_

#include "../../core/Processing.h"
#include "../link/RadioControler.h"
#include "../ahrs/AHRS.h"
#include "../../data/conf/Param.h"

class FlightControl : public Processing {
private:
	RadioControler *_radioController;

	float yawIntegralDesired;

	// Configuration parameters
	Param<float> *maxAbsRollAngle;
	Param<float> *maxAbsPitchAngle;
	Param<float> *maxAbsCombinedAngle;

	// Angle desired (rad)
	float rollDesired;
	float pitchDesired;

	short autoMode;
	Date initDate;

	Date lastExitModeStartDate;
	short exitCommand;

	// Output
	Quaternion attitudeDesired;
	float throttleOut;
public:
	FlightControl(RadioControler*);

	void init();
	void process();
	void callback() { };

	float radioToRad(float, float);

	float rpy[2];

	float getYawInt() {
		return yawIntegralDesired;
	}

	float getRollDesired() {
		return rollDesired;
	}
	float getPitchDesired() {
		return pitchDesired;
	}

	bool isAutoMode() {
		return (autoMode == 1);
	}

	bool isExitCommand()
	{
		return exitCommand == 1;
	}

	float getThrottleOut() {
		return throttleOut;
	}
	Quaternion getAttitudeDesired() {
		return attitudeDesired;
	}

	RadioControler *getRadioControler()
	{
		return _radioController;
	}
};

#endif /* PROCESSING_FLIGHTSTABILIZATION_FLIGHTCONTROL_H_ */
