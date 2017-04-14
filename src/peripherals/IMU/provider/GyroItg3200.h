/*
 * GyroItg3200.h
 *
 *  Created on: May 10, 2016
 *      Author: adrien
 */

#ifndef SRC_PERIPHERALS_IMU_PROVIDER_GYROITG3200_H_
#define SRC_PERIPHERALS_IMU_PROVIDER_GYROITG3200_H_


#include "IMUProvider.h"

class GyroItg3200  : public IMUProvider {
protected:
	I2C i2c;
public:
	GyroItg3200();
	virtual ~GyroItg3200();



	void init();
	Vect3D read();
};

#endif /* SRC_PERIPHERALS_IMU_PROVIDER_GYROITG3200_H_ */
