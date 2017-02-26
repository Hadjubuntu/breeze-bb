/*
 * FlightControl.cpp
 *
 *  Created on: Oct 4, 2015
 *      Author: adrien
 */

#include "../../data/conf/Conf.h"
#include "../../math/common/FastMath.h"
#include "../flightstabilization/FlightStabilization.h"


/**
 * Flight control works on quaternion to prevent from glimbal lock.
 * Reference paper : stabilization using quaternion
 * http://www.nt.ntnu.no/users/skoge/prost/proceedings/ecc-2013/data/papers/0927.pdf
 */

// good values : Pq=20; Pw=3
FlightStabilization::FlightStabilization(AHRS *ahrs, FlightControl *flightControl, Sonar *sonar) :
Processing(),
_targetAttitude(Quaternion::zero()), _currentAttitude(Quaternion::zero()),
_gyroRot(Vect3D::zero()),
_tau(Vect3D::zero())
{
	freqHz = 100;
	_throttle = 0;
	_yawFromGyro = 0.0;
	_Pq = Conf::getInstance().get("flightStabilization_Pq");
	_Pw = Conf::getInstance().get("flightStabilization_Pw");
	_Kangle = Conf::getInstance().get("flightStabilization_Kangle");
	_KrateRoll = Conf::getInstance().get("flightStabilization_KrateRoll");
	_KratePitch = Conf::getInstance().get("flightStabilization_KratePitch");
//	_KrateYaw = Conf::getInstance().get("flightStabilization_KrateYaw");
	_throttleHover = Conf::getInstance().get("flightStabilization_throttleHover");

	// Note that we use radian angles. It means 5 * 0.01 for integral means 2.86° correction for integral terms
	pidRoll.init(_KrateRoll->getValue(), 0.01, 0.05, 2.0);
	pidPitch.init(_KratePitch->getValue(), 0.01, 0.05, 2.0);
	_pidAltitude.init(0.55, 0.04, 0.01, 4);

	_ahrs = ahrs;
	_throttleTarget = 0.0;
	_throttleSlewRate = 0.4; // 50% per second
	_throttleOut = 0.0;
	_flightControl = flightControl;
	_sonar = sonar;
	_meanAccZ = 1.0;
}


void FlightStabilization::updateInputParameters()
{
	_targetAttitude = _flightControl->getAttitudeDesired();
	_throttle = _flightControl->getThrottleOut(); // Throttle is contained between [0; 1]
	//
	_currentAttitude = _ahrs->getAttitude();
	_yawFromGyro = _ahrs->getYawFromGyro();
	_gyroRot = _ahrs->getGyroFiltered();
}


void FlightStabilization::process()
{
	// DEBUG - tricks on yaw

	//#if REAL_VERSION
	//	Vect3D currentAttVect3D = _currentAttitude.toRollPitchYawVect3D();
	//	_currentAttitude = Quaternion(currentAttVect3D.getX(), currentAttVect3D.getY(), _yawFromGyro);
	//
	//	// Compute error from attitude commanded to current attitude using the combined rotation
	//	Quaternion qError = _targetAttitude * (_currentAttitude.conjugate());
	//
	//	// Axis error
	//	Vect3D axisError = qError.getVect3DPart();
	//
	//	// Compute tau from error and gyro rate
	//	_tau = ((axisError * _Pq->getValue()) * (-1)) + ( _gyroRot * _Pw->getValue());
	//
	//	if (Conf::getInstance().useBoostMotors)
	//	{
	//		_throttleOut = boostThrottleCompensateTiltAngle(_throttle);
	//	}
	//	else
	//	{
	//		_throttleOut = _throttle;
	//	}
	//#endif

	updateInputParameters();

	// DEBUG simple PID
	float rpyTarget[3];
	_targetAttitude.toRollPitchYaw(rpyTarget);
	float rpyCurrent[3];
	_currentAttitude.toRollPitchYaw(rpyCurrent);


	// Define angle rate from angle error
	float rollRate = (rpyTarget[0] - rpyCurrent[0]) * _Kangle->getValue();
	float pitchRate = (rpyTarget[1] - rpyCurrent[1]) * _Kangle->getValue();
	float yawRate = (rpyTarget[2] - _yawFromGyro) * _Kangle->getValue();

	BoundAbs(rollRate, 3.14);
	BoundAbs(pitchRate, 3.14);


	pidRoll.setGainParameters(_KrateRoll->getValue(), 0.01, 0.01);
	pidPitch.setGainParameters(_KratePitch->getValue(), 0.01, 0.01);

	pidRoll.update(rollRate - _gyroRot[0], 1/freqHz);
	pidPitch.update(pitchRate - _gyroRot[1], 1/freqHz);

	_tau = Vect3D(pidRoll.getOutput(),
			pidPitch.getOutput(),
			3.0*_KratePitch->getValue() * (yawRate - _gyroRot[2]));

	//	 Control altitude
	//	 ---
	stabilizeAltitude();

}

void FlightStabilization::stabilizeAltitude()
{

	// Manual mode
	if (!_flightControl->isAutoMode()) {

		_pidAltitude.reset();

		if (Conf::getInstance().useBoostMotors)
		{
			_throttleOut = boostThrottleCompensateTiltAngle(_throttle);
		}
		else
		{
			_throttleOut = _throttle;
		}

		if (_throttleOut == 0.0)
		{
			// Recalibrate barometer
			_ahrs->getBaro()->recalibrateAtZeroThrottle();

			// Average mean acceleration in z-axis
			_meanAccZ = 0.7 * _meanAccZ + 0.3 * _ahrs->getAnalyzedAccZ();
		}
	}
	// Auto mode
	else
	{
		if (dt == 0.0) {
			dt = 1.0/freqHz;
		}

		float refAltitudeMeters = _ahrs->getAltitudeMeters();

		if (_sonar->isHealthy() && _sonar->getOutput() < 200)
		{
			refAltitudeMeters = _sonar->getOutput() / 100.0f;
		}
		else
		{
			// Altitude at least equals to max sonar altitude in meters
			refAltitudeMeters = min(2.0, refAltitudeMeters);
		}

		float altitudeSetpointMeters = 1.0; // Test to 150 centimeters
		float errorAltMeters = altitudeSetpointMeters - refAltitudeMeters;
		float accZsetpoint = 0.05 * errorAltMeters;
		Bound(accZsetpoint, -0.1, 0.1);

		float errorAccZ = (accZsetpoint + (_ahrs->getAnalyzedAccZ() - _meanAccZ));
		Bound(errorAccZ, -0.12, 0.12); // 0.2 accZ

		_pidAltitude.update(errorAccZ, dt);

		_throttleTarget = _throttleHover->getValue() + _pidAltitude.getOutput();
		Bound(_throttleTarget, 0.0, 0.67); // Limit to 70% max throttle

		float dThrottle = _throttleTarget - _throttleOut;
		Bound(dThrottle, -_throttleSlewRate / freqHz, _throttleSlewRate / freqHz);

		_throttleOut = _throttleOut + dThrottle;
	}
}

float FlightStabilization::boostThrottleCompensateTiltAngle(float throttle)
{
	Vect3D rpy = _currentAttitude.toRollPitchYawVect3D();

	float combinedTilt = FastMath::fabs(FastMath::fcos(rpy.getX()) * FastMath::fcos(rpy.getY()));
	float factor = 1.0;

	if (isSafeToUseBoost(throttle, combinedTilt))
	{
		factor = 1.0 / combinedTilt;
	}
	Bound(factor, 1.0, 1.5);

	return factor * throttle;
}

bool FlightStabilization::isSafeToUseBoost(float throttle, float combinedTilt)
{
	return throttle > 50 && combinedTilt > 0.0;
}


