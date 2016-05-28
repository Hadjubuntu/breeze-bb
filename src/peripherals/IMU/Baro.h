/*
 * Baro.h
 *
 *  Created on: Oct 11, 2015
 *      Author: adrien
 */

#ifndef PERIPHERALS_IMU_BARO_H_
#define PERIPHERALS_IMU_BARO_H_

#include "../../hal/I2C.h"
#include "../../core/Processing.h"

#define BMP085_ADDRESS 0x77  //(0xEE >> 1)
#define BMP085_EOC 30        // End of conversion pin PC7 on APM1

// See datasheet : https://www.sparkfun.com/datasheets/Components/General/BST-BMP085-DS000-05.pdf

// No EOC connection from Baro
// Use times instead.
// Temp conversion time is 4.5ms
// Pressure conversion time is 25.5ms (for OVERSAMPLING=3)


// oversampling 3 gives 25.5ms sampling time
// oversampling 1 gives 7.5 ms
#define OVERSAMPLING 3

class Baro : public Processing {
private:
	I2C _i2c;
	short dev_address;

	bool firstMeasure;
	long        uncompensatedPressure;
	long        uncompensatedTemperature;
	uint8_t			_count;
	long trueTemperature;
	long truePressure;
	float _altitudeMeters;
	int _iter;

	long b5;

	int                           last_update; // in us
	short                            pressure_samples;
	// State machine
	// Internal calibration registers
	int                        ac1, ac2, ac3, b1, b2, mb, mc, md;
	int                       ac4, ac5, ac6;


	int                       retry_time;

	int _state;
	bool baroHealthy;


	void readUncompensatedTempValue();
	void readUncompensatedPressureValue();
	void calculateTrueTemperature();
	void calculateTruePressure();
	void calculateAltitude();

public:

	long 			GroundPressure;
	long 			GroundTemp;

	Baro();

	void init();
	void process();
	void callback();

	float getAltitudeMeters() {
		return _altitudeMeters;
	}
	long getTrueTemperature() {
		return trueTemperature;
	}
	long getTruePressure() {
		return truePressure;
	}
	long getGroundPressure() {
		return GroundPressure;
	}

	static Baro create() {
		Baro e;
		return e;
	}

	void recalibrateAtZeroThrottle();

};




#endif /* PERIPHERALS_IMU_BARO_H_ */
