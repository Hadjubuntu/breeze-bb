/*
 * Sensor.h
 *
 *  Created on: Nov 10, 2015
 *      Author: adrien-i3
 */

#ifndef PERIPHERALS_SENSOR_SENSOR_H_
#define PERIPHERALS_SENSOR_SENSOR_H_

#include "../../hal/I2C.h"
#include "../../math/vector/Vect3D.h"

class Sensor {
protected:
	/** I2C communication */
	I2C _i2c;

	/** Filtered data */
	Vect3D _dataFiltered;

	/** Raw data */
	Vect3D _dataRaw;

	/** Initial offset */
	Vect3D _offset;

	/** Filter coefficient */
	float _filterNewDataCoeff;
public:
	Sensor(I2C pI2C) : _i2c(pI2C),
	_dataFiltered(Vect3D::zero()),
	_dataRaw(Vect3D::zero()),
	_offset(Vect3D::zero()),
	_filterNewDataCoeff(0.5f)
{

}

	virtual void init() = 0;

	virtual void update() = 0;

	virtual ~Sensor()
	{

	}
};

#endif /* PERIPHERALS_SENSOR_SENSOR_H_ */
