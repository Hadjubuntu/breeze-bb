/*
 * AccMPU6050.h
 *
 *  Created on: May 10, 2016
 *      Author: adrien
 */

#ifndef SRC_PERIPHERALS_IMU_PROVIDER_ACCMPU6050_H_
#define SRC_PERIPHERALS_IMU_PROVIDER_ACCMPU6050_H_

#include "IMUProvider.h"

class AccMPU6050  : public IMUProvider  {
public:
	AccMPU6050();
	virtual ~AccMPU6050();


	void init();
	Vect3D read();
};

#endif /* SRC_PERIPHERALS_IMU_PROVIDER_ACCMPU6050_H_ */
