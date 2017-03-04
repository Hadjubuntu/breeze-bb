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

FlightControl::FlightControl(RadioControler *radioController) : Processing(),
	initDate(Date::zero()),
	attitudeDesired(Quaternion::zero()),
	lastExitModeStartDate(Date::zero())
{
	_radioController = radioController;

	yawIntegralDesired = 0.0;

	// Runs at 50Hz
	freqHz = 50;

	// Retrieve conf parameters
	maxAbsRollAngle = Conf::getInstance().get("maxAbsRollAngle");
	maxAbsPitchAngle = Conf::getInstance().get("maxAbsPitchAngle");
	maxAbsCombinedAngle = Conf::getInstance().get("maxAbsCombinedAngle");

	rollDesired = 0.0;
	pitchDesired = 0.0;
	yawIntegralDesired = 0.0;

	autoMode = 0;
	throttleOut = 0.0;

	exitCommand = 0;
	awaitExit = 0;
}

/**
 * Initialize flight controller
 */
void FlightControl::init()
{
	initDate = Date::now();
}

void FlightControl::process()
{
	// Check radio state mode: auto or manual
	//	_radioController->isAutoMode();

	// AUTO mode
	// ------------------
	float currentAutoMode = _radioController->getHandler().getChannelNormed(6);

	// Do not allow auto mode before 30 seconds from power-up
	if (Date::now().durationFrom(initDate) > 30.0) {
		if (currentAutoMode > 0.5) {
			autoMode = 1;
		}
		else {
			autoMode = 0;
		}
	}


	// Update mission navigation

	// MANUAL mode
	// ------------------

	// Compute roll, pitch, yaw desired by using the radio values
	rollDesired = radioToRad(_radioController->getRollCommandNormed(), maxAbsRollAngle->getValue());
	pitchDesired = radioToRad(_radioController->getPitchCommandNormed(), maxAbsPitchAngle->getValue());
	float yaw = radioToRad(_radioController->getYawCommandNormed(), maxAbsCombinedAngle->getValue());
	// Throttle from 0 to 1
	float throttle = _radioController->getThrottleCommandNormed();
	Bound(throttle, 0.0, 1.0);

	// Integrate desired yaw
	const float Kyaw = 4.0;
	yawIntegralDesired += Kyaw * yaw * 1/freqHz;
	yawIntegralDesired = yawIntegralDesired * 0.96;
	Bound(yawIntegralDesired, -PI, PI); //FIXME when max left and turns left, go other side ..

	// Transform RPY to quaternion
	attitudeDesired = Quaternion::fromEuler(rollDesired, pitchDesired, yawIntegralDesired);

	throttleOut = throttle;


	// To quit breeze application, when user input yaw max + roll max more than 5 seconds, then quit is commanded
	printf("roll cmd %.2f | yaw cmd %.2f\n", FastMath::fabs(_radioController->getRollCommandNormed()), FastMath::fabs(_radioController->getYawCommandNormed()));

	if (throttle < 0.1 && FastMath::fabs(_radioController->getRollCommandNormed()) > 0.6 && FastMath::fabs(_radioController->getYawCommandNormed()) > 0.6)
	{
		if (awaitExit == 0)
		{
			lastExitModeStartDate = Date::now();
			awaitExit = 1;
		}
		else {
			if (Date::now().durationFrom(lastExitModeStartDate) > 5.0)
			{
				exitCommand = 1;
				awaitExit = 0;
			}
		}
	}
	else {
		lastExitModeStartDate = Date::zero();
		awaitExit = 0;
		exitCommand = 0;
	}
}


/**
 * Converts radio signal to angle in radians.
 * Radio signal must be contained in [-1; 1] interval.
 */
float FlightControl::radioToRad(float radioNormed, float maxAngle)
{
	return radioNormed * maxAngle;
}
