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

#define GYRO_ADDR 0x68
#define GYRO_SMPLRT_DIV 0x15
#define GYRO_PWR_MNGT 0x3e
#define GYRO_DLPF_FS 0x16
#define GYRO_INT_CFG 0x17
#define GYRO_REG_ADDR 0x1d
#define GYRO_LSB_TO_DEGS 0.0695652174f

class Gyro {
private:
	/** I2C communication */
	I2C _i2c;

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
		_i2c(I2C::getInstance(GYRO_ADDR)),
		_gyroRaw(Vect3D::zero()),
		_gyroFiltered(Vect3D::zero()),
		_offset(Vect3D::zero()),
		_filterNewDataCoeff(0.5) {

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
