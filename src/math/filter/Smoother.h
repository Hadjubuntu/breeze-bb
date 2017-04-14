/*
 * Smoother.h
 *
 *  Created on: Mar 25, 2017
 *      Author: adrien
 */

#ifndef SRC_MATH_FILTER_SMOOTHER_H_
#define SRC_MATH_FILTER_SMOOTHER_H_

#include "../time/Date.h"

class Smoother {
private:
	float target;
	float current;
	float dcurrent;
	Date lastCall;
public:
	Smoother(float, float);

	float getCurrent();

	void setParameters(float pCurrent, float pTarget, float pDcurrent)
	{
		current = pCurrent;
		target = pTarget;
		dcurrent = pDcurrent;
	}

	/**
	 * Setters
	 */
	void setTarget(float pTarget)
	{
		target = pTarget;
	}


	virtual ~Smoother();
};

#endif /* SRC_MATH_FILTER_SMOOTHER_H_ */
