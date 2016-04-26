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

////////Acceleration sensor ADXL345 function/////////////////////////////
#define ACC (0x53)    // Defined ADXL345 address, ALT ADDRESS pin is grounded
#define A_TO_READ (6) // Number of bytes to read(each axis accounted for two-byte)
#define XL345_DEVID   0xE5 // ADXL345 ID register
// ADXL345 Control register
#define ADXLREG_TAP_AXES     0x2A
#define ADXLREG_BW_RATE      0x2C
#define ADXLREG_POWER_CTL    0x2D
#define ADXLREG_INT_ENABLE   0x2E
#define ADXLREG_DATA_FORMAT  0x31
#define ADXLREG_FIFO_CTL     0x38
#define ADXLREG_DUR          0x21

//ADXL345 Data register
#define ADXLREG_DEVID        0x00
#define ADXLREG_DATAX0       0x32
#define ADXLREG_DATAX1       0x33
#define ADXLREG_DATAY0       0x34
#define ADXLREG_DATAY1       0x35
#define ADXLREG_DATAZ0       0x36
#define ADXLREG_DATAZ1       0x37

#define ACC_SENSITIVITY 0.003906f


class Accelerometer : public Sensor
{
private:
	/** Internal logger */
	Logger logger;
	/** Filtered acceleration */
	Vect3D accFiltered;
	/** Raw data acceleration */
	Vect3D accRaw;
public:
	/**
	 * Constructor
	 */
	Accelerometer() :
		Sensor(I2C::getInstance(ACC)),
		logger(Logger::getDefault()),
		accFiltered(Vect3D::zero()),
		accRaw(Vect3D::zero()) {

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
