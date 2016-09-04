/*
 * FlightControl.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: adrien
 */

#include "FlightControl.h"

#include "../../hal/HalMath.h"
#include "../../data/conf/Conf.h"
#include "../../math/common/FastMath.h"

FlightControl::FlightControl(RadioControler *radioController) : Processing(), _initDate(Date::zero()), _attitudeDesired(Quaternion::zero())
{
	_radioController = radioController;

	_yawInt = 0.0;

	// Runs at 50Hz
	freqHz = 50;
	_throttleInitUs = 370;

	// Retrieve conf parameters
	_maxAbsRollAngle = Conf::getInstance().get("maxAbsRollAngle");
	_maxAbsPitchAngle = Conf::getInstance().get("maxAbsPitchAngle");
	_maxAbsCombinedAngle = Conf::getInstance().get("maxAbsCombinedAngle");

	_rollDesired = 0.0;
	_pitchDesired = 0.0;
	_yawInt = 0.0;

	_auto = 0;
	_throttleOut = 0.0;
}

/**
 * Initialize flight controller
 */
void FlightControl::init()
{
	// Init throttle at minimum value [us]
	//TODO wait until value between 300 and 500 ..
	_throttleInitUs = _radioController->getHandler().Channel(3);

	_initDate = Date::now();
}

void FlightControl::process()
{
	// Check radio state mode: auto or manual
	//	_radioController->isAutoMode();

	// AUTO mode
	// ------------------
	float currentAutoMode = _radioController->getHandler().getChannelNormed(7);

	// Do not allow auto mode before 30 seconds from power-up
	if (Date::now().durationFrom(_initDate) > 30.0) {
		if (currentAutoMode > 0.5) {
			_auto = 1;
		}
		else {
			_auto = 0;
		}
	}


	// Update mission navigation

	// MANUAL mode
	// ------------------

	// Compute roll, pitch, yaw desired by using the radio values
	_rollDesired = radioToRad(_radioController->getHandler().getChannelNormed(1), _maxAbsRollAngle->getValue());
	_pitchDesired = radioToRad(_radioController->getHandler().getChannelNormed(2), _maxAbsPitchAngle->getValue());
	float yaw = radioToRad(_radioController->getHandler().getChannelNormed(4), _maxAbsCombinedAngle->getValue());
	// Throttle from 0 to 1
	float throttle = (_radioController->getHandler().Channel(3) - _throttleInitUs) / 1310.0;
	Bound(throttle, 0.0, 1.0);

	// Integrate desired yaw
	const float Kyaw = 4.0;
	_yawInt += Kyaw * yaw * 1/freqHz;
	_yawInt = _yawInt * 0.96;
	Bound(_yawInt, -PI, PI); //FIXME when max left and turns left, go other side ..

	// Transform RPY to quaternion
	_attitudeDesired = Quaternion::fromEuler(_rollDesired, _pitchDesired, _yawInt);

	_throttleOut = throttle;
}


/**
 * Converts radio signal to angle in radians.
 * Radio signal must be contained in [-1; 1] interval.
 */
float FlightControl::radioToRad(float radioNormed, float maxAngle)
{
	return radioNormed * maxAngle;
}
