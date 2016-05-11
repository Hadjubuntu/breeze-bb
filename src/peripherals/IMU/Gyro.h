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
#include "provider/GyroProvider.h"
#include "provider/GyroMPU6050.h"

class Gyro {
private:
	/** Provider */
	GyroProvider *provider;

	/** Raw data acceleration */
	Vect3D _gyroRaw;

	/** Filtered gyro */
	Vect3D _gyroFiltered;

	/** Initial offset acceleration */
	Vect3D _offset;

	/** Filter coefficient */
	float _filterNewDataCoeff;

public:
	/**
	 * Constructor
	 */
	Gyro() :
		_gyroRaw(Vect3D::zero()),
		_gyroFiltered(Vect3D::zero()),
		_offset(Vect3D::zero()),
		_filterNewDataCoeff(0.5)
	{
		// Initialize provider
		GyroMPU6050 providerMPU6050;
		provider = &providerMPU6050;
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
	Vect3D getGyroRaw() { return _gyroRaw; }
	Vect3D getGyroFiltered() { return _gyroFiltered; }

	/****************************************************
	 * Instance
	 ***************************************************/
	static Gyro create() {
		Gyro e;
		return e;
	}
};

#endif /* PERIPHERALS_IMU_GYRO_H_ */
