/*
 * GyroMPU6050.h
 *
 *  Created on: May 10, 2016
 *      Author: adrien
 */

#ifndef SRC_PERIPHERALS_IMU_PROVIDER_GYROMPU6050_H_
#define SRC_PERIPHERALS_IMU_PROVIDER_GYROMPU6050_H_

#include "GyroProvider.h"

class GyroMPU6050 : public GyroProvider {
protected:
	I2C i2c;
public:
	GyroMPU6050();
	virtual ~GyroMPU6050();

	void init();
	Vect3D read();
};

#endif /* SRC_PERIPHERALS_IMU_PROVIDER_GYROMPU6050_H_ */
