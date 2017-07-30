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
AHRS::AHRS(Baro *baro) : Processing(), grot(Vect3D::zero()),
		attitude(Quaternion::zero()),
		attitudeOffset(Quaternion::zero()),
		gyro(Gyro::create()),
		accelerometer(Accelerometer::create()),
		yawFromGyro(0.0),
		lastPositiveAccPeak(Date::now()),
		lastNegativeAccPeak(Date::now()),
		lastAttitude(Quaternion::zero()),
		lastAttitudeDateStored(Date::now()),
		gyroHyperFilted(Vect3D::zero())
//		_baro(Baro::create())
{
	// 400 Hz update
	freqHz = 400;

	gyro_correct_int[0] = 0.0;
	gyro_correct_int[1] = 0.0;

	meanAccZ = 0.0;
	itrAccZ = 0;
	vZ = 0.0;
	analyzedAccZ = 0.0;
	barometer = baro;
}

void AHRS::calibrateOffset()
{
	float rpy[3];
	attitude.toRollPitchYaw(rpy);
	attitudeOffset = Quaternion(-rpy[0], -rpy[1], 0.0);
}

void AHRS::init()
{
	// Initialize gyro first
	gyro.init();
	accelerometer.init();
}

float* AHRS::getGyroCorr() {
	return gyro_correct_int;
}



void AHRS::process()
{
	const float accelKi = 0.000174;
	const float accelKp = 0.00174;
	const float rollPitchBiasRate = 0.999999;

	accelerometer.update();
	gyro.update();

	// Retrieve Gyro
	Vect3D tmpGyros = gyro.getGyroFiltered();
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
	arrayGrot[0] = -(2.0 * (attitude[1] * attitude[3] - attitude[0] * attitude[2]));
	arrayGrot[1] = -(2.0 * (attitude[2] * attitude[3] + attitude[0] * attitude[1]));
	arrayGrot[2] = -(attitude[0] * attitude[0] - attitude[1] * attitude[1] - attitude[2] * attitude[2] + attitude[3] * attitude[3]);
	grot = Vect3D(arrayGrot[0], arrayGrot[1], arrayGrot[2]);


	// filter g rot
	Vect3D accelError = accelerometer.getAccFiltered().crossProduct(grot);

	float invAccelMag = FastMath::fast_invsqrtf(accelerometer.getAccFiltered().getNorm2());

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
	qdot[0] = (-attitude[1] * gyros[0] - attitude[2] * gyros[1] - attitude[3] * gyros[2]) * dt *  0.5f;
	qdot[1] = (attitude[0] * gyros[0] - attitude[3] * gyros[1] + attitude[2] * gyros[2]) * dt  * 0.5f;
	qdot[2] = (attitude[3] * gyros[0] + attitude[0] * gyros[1] - attitude[1] * gyros[2]) * dt * 0.5f;
	qdot[3] = (-attitude[2] * gyros[0] + attitude[1] * gyros[1] + attitude[0] * gyros[2]) * dt * 0.5f;

	// Take a time step
	attitude += qdot;

	if (attitude[0] < 0) {
		attitude = -attitude;
	}

	float inv_qmag = FastMath::fast_invsqrtf(attitude.getNorm2());

	// If quaternion has become inappropriately short or is nan reinit.
	// THIS SHOULD NEVER ACTUALLY HAPPEN
	if ((FastMath::fabs(inv_qmag) > 1e3f) || isnan(inv_qmag)) {
		attitude = Quaternion::zero();
		logger.error("inv_qmag > 1e3f");
	} else {
		attitude *= inv_qmag;
	}

	// Integrate gyro rotation Z to have an estimation of the yaw
	yawFromGyro += gyros.getZ() / freqHz;
	yawFromGyro = 0.9 * yawFromGyro;
	yawFromGyro = FastMath::constrainAngleMinusPiPlusPi(yawFromGyro);

	// Store attitude and delta
	float dtAttitude = Date::now().durationFrom(lastAttitudeDateStored);
	// TODO improve ..
	if (dtAttitude > 0.01)
	{
		 gyroHyperFilted = (attitude.toRollPitchYawVect3D() - lastAttitude.toRollPitchYawVect3D()) / dtAttitude;
		lastAttitude = attitude;
		lastAttitudeDateStored = Date::now();
	}

	// Integrate delta accZ to have estimation on vertical speed
	computeVz();
}

void AHRS::computeVz()
{
	Date now = Date::now();
	Vect3D acc = accelerometer.getAccFiltered();
	Vect3D acc_Ef = attitude.conjugate().rotate(acc);

	// Compute average for 2 seconds
	if (itrAccZ < freqHz * 2) {
		if (itrAccZ == 0) {
			meanAccZ = acc_Ef.getZ();
		}
		else {
			meanAccZ = 0.8 * meanAccZ + 0.2 * acc_Ef.getZ();
		}

		itrAccZ ++;
	}

	float Kd = 0.99;

	analyzedAccZ = acc_Ef.getZ() - meanAccZ;

	// Filter low value with no-tolerance (set value to zero)
	if (FastMath::fabs(analyzedAccZ) < 0.05) {
		analyzedAccZ = 0.0;
	}
	// Filter low value with tolerance
	else if (FastMath::fabs(analyzedAccZ) < 0.1) {
		analyzedAccZ = analyzedAccZ / 4.0;
	}

	float factorPeak = 1.0;

	// Store date of peak acceleration in z body-fame value
	if (analyzedAccZ > 0.3) {
		lastPositiveAccPeak = Date::now();
	}
	else if (analyzedAccZ < -0.3) {
		lastNegativeAccPeak = Date::now();
	}
	else {
		// Low accZ
		Kd = 0.98;
	}

	// Filter counter-peak except if fast change of peak sign
	if (analyzedAccZ > 0.0
			&& now.durationFrom(lastNegativeAccPeak) < 1.0
			&& now.durationFrom(lastPositiveAccPeak) >= 2.0) {
		factorPeak = 0.05;
		Kd = 0.9;
	}
	else 	if (analyzedAccZ < 0.0
			&& now.durationFrom(lastPositiveAccPeak) < 1.0
			&& now.durationFrom(lastNegativeAccPeak) >= 2.0) {
		factorPeak = 0.05;
		Kd = 0.9;
	}

	analyzedAccZ = analyzedAccZ * factorPeak;

	// TOdo increase kd if long term sign.. decrease when freeze
	vZ = Kd * (vZ + 9.81*analyzedAccZ * dt);
}
