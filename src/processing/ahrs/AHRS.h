/*
 * AHRS.h
 *
 * Attitude and heading reference system
 *
 *  Created on: Sep 22, 2015
 *      Author: adrien
 */

#ifndef PROCESSING_AHRS_AHRS_H_
#define PROCESSING_AHRS_AHRS_H_


#include "../../core/Processing.h"
#include "../../math/vector/Quaternion.h"
#include "../../math/common/FastMath.h"
#include "../../peripherals/IMU/Accelerometer.h"
#include "../../peripherals/IMU/Baro.h"
#include "../../peripherals/IMU/Gyro.h"


class AHRS : public Processing {
private:

	/** Rotation gravity body frame */
	Vect3D grot;

	/** Quaternion of attitude */
	Quaternion attitude;

	/** Quaternion of attitude offset */
	Quaternion attitudeOffset;

	/** Gyroscope */
	Gyro gyro;

	/** Accelerometer */
	Accelerometer accelerometer;

	/** Barometer */
	Baro *barometer;

	// Yaw computed from gyro integration
	float yawFromGyro;

	Date lastPositiveAccPeak;
	Date lastNegativeAccPeak;

	int itrAccZ;
	float meanAccZ;
	float vZ;
	float analyzedAccZ;

	Date lastAttitudeDateStored;
	Quaternion lastAttitude;
	Vect3D gyroHyperFilted;

public:
	AHRS(Baro *);

	/**
	 * Initialize sensors accelerometer and gyro
	 */
	void init();

	/**
	 * Process and update data
	 */
	void process();
	void computeVz();
	void callback() { };

	float* getGyroCorr();

	void calibrateOffset();


	Vect3D getGyroHyperFiltered()
	{
		return gyroHyperFilted;
	}

	/******************************************************
	 * GETTERS
	 *****************************************************/

	Vect3D getGyroFiltered() {
		return gyro.getGyroFiltered();
	}

	Accelerometer getAcc() { return accelerometer; }
	Gyro getGyro() { return gyro; }

	Quaternion getAttitude() { return attitude * attitudeOffset; }

	float getYawFromGyro() {
		return yawFromGyro;
	}

	float getVz() {
		return vZ;
	}

	float getAnalyzedAccZ() {
		return analyzedAccZ;
	}

	float getAltitudeMeters()
	{
		return barometer->getAltitudeMeters();
	}

	Baro* getBaro() {
		return barometer;
	}

	void printOffset()
	{
		float rpy[3];
		attitudeOffset.toRollPitchYaw(rpy);
		printf("Offset attitude=%.2f, %.2f, %.2f\n",
				FastMath::toDegrees(rpy[0]),
				FastMath::toDegrees(rpy[1]),
				FastMath::toDegrees(rpy[2]));
	}

};

#endif /* PROCESSING_AHRS_AHRS_H_ */
