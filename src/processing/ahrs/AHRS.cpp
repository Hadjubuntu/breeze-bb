/*
 * AHRS.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: adrien
 */

#include <stdio.h>
#include "../../math/common/FastMath.h"
#include "AHRS.h"


float gyro_correct_int[3];


/**
 * Constructor
 */
AHRS::AHRS(Baro *baro) : Processing(), _grot(Vect3D::zero()),
		_attitude(Quaternion::zero()),
		_attitudeOffset(Quaternion::zero()),
		_gyro(Gyro::create()),
		_accelerometer(Accelerometer::create()),
		_yawFromGyro(0.0),
		_lastPositiveAccPeak(Date::now()),
		_lastNegativeAccPeak(Date::now())
//		_baro(Baro::create())
{
	// 400 Hz update
	freqHz = 400;

	gyro_correct_int[0] = 0.0;
	gyro_correct_int[1] = 0.0;

	_meanAccZ = 0.0;
	_itrAccZ = 0;
	_vZ = 0.0;
	_analyzedAccZ = 0.0;
	_baro = baro;
	healthy = false;
}

void AHRS::calibrateOffset()
{
	float rpy[3];
	_attitude.toRollPitchYaw(rpy);
	_attitudeOffset = Quaternion(-rpy[0], -rpy[1], 0.0);
}

void AHRS::init()
{
	// Initialize gyro first
	_gyro.init();
	_accelerometer.init();
}

float* AHRS::getGyroCorr() {
	return gyro_correct_int;
}



void AHRS::process()
{
	const float accelKi = 0.000174;
	const float accelKp = 0.00174;
	const float rollPitchBiasRate = 0.999999;


	_accelerometer.update();
	_gyro.update();

	// Retrieve Gyro
	Vect3D tmpGyros = _gyro.getGyroFiltered();
	// Correct value
	Vect3D gyros = Vect3D(tmpGyros.getX() + gyro_correct_int[0], tmpGyros.getY() + gyro_correct_int[1], tmpGyros.getZ());

	// Zeroing error
	// Force the roll & pitch gyro rates to average to zero
	gyro_correct_int[0] *= rollPitchBiasRate;
	gyro_correct_int[1] *= rollPitchBiasRate;

	// TODO as https://github.com/openpilot/OpenPilot/blob/master/flight/modules/Attitude/attitude.c
	//---------------------

	// Compute rotation
	float arrayGrot[3];
	arrayGrot[0] = -(2.0 * (_attitude[1] * _attitude[3] - _attitude[0] * _attitude[2]));
	arrayGrot[1] = -(2.0 * (_attitude[2] * _attitude[3] + _attitude[0] * _attitude[1]));
	arrayGrot[2] = -(_attitude[0] * _attitude[0] - _attitude[1] * _attitude[1] - _attitude[2] * _attitude[2] + _attitude[3] * _attitude[3]);
	_grot = Vect3D(arrayGrot[0], arrayGrot[1], arrayGrot[2]);


	// filter g rot
	Vect3D accelError = _accelerometer.getAccFiltered().crossProduct(_grot);

	float invAccelMag = FastMath::fast_invsqrtf(_accelerometer.getAccFiltered().getNorm2());

	if (invAccelMag > 1e3f) {
		return;
	}
	float  invGRotMag = FastMath::fast_invsqrtf(gyros.getNorm2());

	if (invGRotMag > 1e3f) {
		return;
	}
	const float invMag = (invAccelMag * invGRotMag);
	accelError *= invMag;

	// Accumulate integral of error.  Scale here so that units are (deg/s) but Ki has units of s
	gyro_correct_int[0] += accelError[0] * accelKi;
	gyro_correct_int[1] += accelError[1] * accelKi;


	// Correct rates based on error, integral component dealt with in updateSensors
	if (dt > 0.0) {
		const float kpInvdT = accelKp / dt;
		gyros += (accelError * kpInvdT);
	}

	float qdot[4];
	qdot[0] = (-_attitude[1] * gyros[0] - _attitude[2] * gyros[1] - _attitude[3] * gyros[2]) * dt *  0.5f;
	qdot[1] = (_attitude[0] * gyros[0] - _attitude[3] * gyros[1] + _attitude[2] * gyros[2]) * dt  * 0.5f;
	qdot[2] = (_attitude[3] * gyros[0] + _attitude[0] * gyros[1] - _attitude[1] * gyros[2]) * dt * 0.5f;
	qdot[3] = (-_attitude[2] * gyros[0] + _attitude[1] * gyros[1] + _attitude[0] * gyros[2]) * dt * 0.5f;

	// Take a time step
	_attitude += qdot;

	if (_attitude[0] < 0) {
		_attitude = -_attitude;
	}

	float inv_qmag = FastMath::fast_invsqrtf(_attitude.getNorm2());

	// If quaternion has become inappropriately short or is nan reinit.
	// THIS SHOULD NEVER ACTUALLY HAPPEN
	if ((FastMath::fabs(inv_qmag) > 1e3f) || isnan(inv_qmag)) {
		_attitude = Quaternion::zero();
		logger.error("inv_qmag > 1e3f");
	} else {
		_attitude *= inv_qmag;
	}

	// Integrate gyro rotation Z to have an estimation of the yaw
	_yawFromGyro += gyros.getZ() / freqHz;
	_yawFromGyro = 0.9925 * _yawFromGyro;
	_yawFromGyro = FastMath::constrainAngleMinusPiPlusPi(_yawFromGyro);

	// Integrate delta accZ to have estimation on vertical speed
	computeVz();

	healthy = true;
}

void AHRS::computeVz()
{
	Date now = Date::now();
	Vect3D acc = _accelerometer.getAccFiltered();
	Vect3D acc_Ef = _attitude.conjugate().rotate(acc);

	// Compute average for 2 seconds
	if (_itrAccZ < freqHz * 2) {
		if (_itrAccZ == 0) {
			_meanAccZ = acc_Ef.getZ();
		}
		else {
			_meanAccZ = 0.8 * _meanAccZ + 0.2 * acc_Ef.getZ();
		}

		_itrAccZ ++;
	}

	float Kd = 0.99;

	_analyzedAccZ = acc_Ef.getZ() - _meanAccZ;

	// Filter low value with no-tolerance (set value to zero)
	if (FastMath::fabs(_analyzedAccZ) < 0.05) {
		_analyzedAccZ = 0.0;
	}
	// Filter low value with tolerance
	else if (FastMath::fabs(_analyzedAccZ) < 0.1) {
		_analyzedAccZ = _analyzedAccZ / 4.0;
	}

	float factorPeak = 1.0;

	// Store date of peak acceleration in z body-fame value
	if (_analyzedAccZ > 0.3) {
		_lastPositiveAccPeak = Date::now();
	}
	else if (_analyzedAccZ < -0.3) {
		_lastNegativeAccPeak = Date::now();
	}
	else {
		// Low accZ
		Kd = 0.98;
	}

	// Filter counter-peak except if fast change of peak sign
	if (_analyzedAccZ > 0.0
			&& now.durationFrom(_lastNegativeAccPeak) < 1.0
			&& now.durationFrom(_lastPositiveAccPeak) >= 2.0) {
		factorPeak = 0.05;
		Kd = 0.9;
	}
	else 	if (_analyzedAccZ < 0.0
			&& now.durationFrom(_lastPositiveAccPeak) < 1.0
			&& now.durationFrom(_lastNegativeAccPeak) >= 2.0) {
		factorPeak = 0.05;
		Kd = 0.9;
	}

	_analyzedAccZ = _analyzedAccZ * factorPeak;

	// TOdo increase kd if long term sign.. decrease when freeze
	_vZ = Kd * (_vZ + 9.81*_analyzedAccZ * dt);
}
