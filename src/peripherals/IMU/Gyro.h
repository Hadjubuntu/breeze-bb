/*
 * Gyro.h
 *
 *  Created on: Sep 22, 2015
 *      Author: adrien
 */

#ifndef PERIPHERALS_IMU_GYRO_H_
#define PERIPHERALS_IMU_GYRO_H_


#include "../../math/vector/Vect3D.h"
#include "../../hal/I2C.h"
#include "../../math/time/Date.h"
#include "provider/GyroMPU6050.h"
#include "provider/IMUProvider.h"

class Gyro {
private:
	/** Provider */
	GyroMPU6050 provider;

	/** Raw data acceleration */
	Vect3D gyroRaw;

	/** Filtered gyro */
	Vect3D gyroFiltered;

	/** Initial offset acceleration */
	Vect3D offset;

	/** Filter coefficient */
	float filterNewDataCoeff;

public:
	/**
	 * Constructor
	 */
	Gyro() :
		gyroRaw(Vect3D::zero()),
		gyroFiltered(Vect3D::zero()),
		offset(Vect3D::zero()),
		filterNewDataCoeff(0.5)
	{

	}

	/**
	 * Initialize sensor
	 */
	void init();

	/**
	 * Update
	 */
	void update();


	/****************************************************
	 * GETTERS
	 ***************************************************/
	Vect3D getGyroRaw() { return gyroRaw; }
	Vect3D getGyroFiltered() { return gyroFiltered; }

	/****************************************************
	 * Instance
	 ***************************************************/
	static Gyro create() {
		Gyro e;
		return e;
	}
};

#endif /* PERIPHERALS_IMU_GYRO_H_ */
