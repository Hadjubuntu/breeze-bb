/*
 * Accelerometer.h
 *
 *  Created on: Sep 22, 2015
 *      Author: adrien
 */

#ifndef PERIPHERALS_IMU_ACCELEROMETER_H_
#define PERIPHERALS_IMU_ACCELEROMETER_H_

#include "../../core/Logger.h"
#include "../../peripherals/sensor/Sensor.h"
#include "../../hal/I2C.h"
#include "provider/AccMPU6050.h"




class Accelerometer
{
private:
	/** Provider */
	AccMPU6050 provider;
	/** Internal logger */
	Logger logger;
	/** Filtered acceleration */
	Vect3D accFiltered;
	/** Raw data acceleration */
	Vect3D accRaw;
	/** Initial offset acceleration */
	Vect3D offset;
	/** Filter coefficient */
	float filterNewDataCoeff;
public:
	/**
	 * Constructor
	 */
	Accelerometer() :
		logger(Logger::getDefault()),
		accFiltered(Vect3D::zero()),
		accRaw(Vect3D::zero()),
		offset(Vect3D::zero()),
		filterNewDataCoeff(0.5f) {

	}

	/** Initialize accelerometer */
	void init();

	/** Proceed to calibration */
	void calibration();

	/** Read new data sensor */
	void update();

	/**
	 * Create a new accelerometer instance
	 */
	static Accelerometer create() {
		Accelerometer e;
		return e;
	}


	/****************************************************
	 * GETTERS
	 ***************************************************/
	Vect3D getAccRaw() {
		return accRaw;
	}
	Vect3D getAccFiltered() {
		return accFiltered;
	}
};

#endif /* PERIPHERALS_IMU_ACCELEROMETER_H_ */
