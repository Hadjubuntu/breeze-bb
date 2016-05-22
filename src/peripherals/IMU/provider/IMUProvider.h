/*
 * GyroProvider.h
 *
 *  Created on: May 10, 2016
 *      Author: adrien
 */

#ifndef SRC_PERIPHERALS_IMU_PROVIDER_IMUPROVIDER_H_
#define SRC_PERIPHERALS_IMU_PROVIDER_IMUPROVIDER_H_


#include "../../../hal/I2C.h"
#include "../../../hal/HAL.h"
#include "../../../math/vector/Vect3D.h"

class IMUProvider {
protected:
public:
	IMUProvider();
	virtual ~IMUProvider();

	virtual void init() = 0;
	virtual Vect3D read() = 0;
};

#endif /* SRC_PERIPHERALS_IMU_PROVIDER_IMUPROVIDER_H_ */
