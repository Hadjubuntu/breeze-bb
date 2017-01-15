/*
 * FlightControl.h
 *
 *  Created on: Oct 4, 2015
 *      Author: adrien
 */

#ifndef PROCESSING_FLIGHTSTABILIZATION_FLIGHTSTABILIZATION_H_
#define PROCESSING_FLIGHTSTABILIZATION_FLIGHTSTABILIZATION_H_

#include "../../math/vector/Quaternion.h"
#include "../../core/Processing.h"
#include "../../data/conf/Param.h"
#include "../../math/pid/PID.h"
#include "../ahrs/AHRS.h"
#include "../flightstabilization/FlightControl.h"
#include "../nav/sonar/Sonar.h"



class FlightStabilization : public Processing {
private:
	// Dependencies
	// ---
	AHRS *_ahrs;
	FlightControl *_flightControl;
	Sonar *_sonar;

	// Inputs
	// ------------------
	Param<float> *_Pq;
	Param<float> *_Pw;
	Param<float> *_Kangle;
	Param<float> *_KratePitch;
	Param<float> *_KrateRoll;
	Param<float> *_KrateYaw;
	Quaternion _targetAttitude;
	Quaternion _currentAttitude;
	Vect3D _gyroRot;
	float _throttle;
	float _yawFromGyro;
	// Outputs
	// ------------------
	Vect3D _tau;
	float _throttleOut;

	// PID
	PID pidRoll;
	PID pidPitch;
	PID _pidAltitude;

	Param<float> *_throttleHover;
	float _throttleTarget;
	float _throttleSlewRate;
	float _meanAccZ;


public:

	/**
	 * Constructor
	 */
	FlightStabilization(AHRS*, FlightControl*, Sonar*);

	/**
	 * Initialization
	 */
	void init() {

	}

	void updateInputParameters();

	/**
	 * Process and update data
	 */
	void process();
	void callback() {};

	void stabilizeAltitude();

	float boostThrottleCompensateTiltAngle(float throttle);
	bool isSafeToUseBoost(float throttle, float combinedTilt);

	/* ****************************
	 * GETTERS
	 *************************** */
	Vect3D getTau() {
		return _tau;
	}

	float getThrottle() {
		return _throttleOut;
	}

	Quaternion getTargetAttitude() {
		return _targetAttitude;
	}

	// Debug
	float getErrorAltitude() {
		return _pidAltitude.getError();
	}

	PID getPidRoll() {
		return pidRoll;
	}

	PID getPidPitch() {
		return pidPitch;
	}
};

#endif /* PROCESSING_FLIGHTSTABILIZATION_FLIGHTSTABILIZATION_H_ */
