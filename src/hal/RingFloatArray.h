/*
 * RingFloatArray.h
 *
 *  Created on: Oct 16, 2016
 *      Author: adrien
 */

#ifndef SRC_HAL_RINGFLOATARRAY_H_
#define SRC_HAL_RINGFLOATARRAY_H_

class RingFloatArray {
private:
	int size;
	int posIdx;
	float values[100];
public:
	RingFloatArray(int pSize);
	void add(float pElement);
	float integralValue();
	float absIntegralValue();
};

#endif /* SRC_HAL_RINGFLOATARRAY_H_ */
