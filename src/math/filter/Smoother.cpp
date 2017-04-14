/*
 * Smoother.cpp
 *
 *  Created on: Mar 25, 2017
 *      Author: adrien
 */


#include "../common/FastMath.h"
#include "Smoother.h"


Smoother::Smoother(float pCurrent, float pDcurrent) : lastCall(Date::zero())
{
	current = pCurrent;
	target = pCurrent;
	dcurrent = pDcurrent;
}

float Smoother::getCurrent()
{
	Date dateNow = Date::now();

	float dt = dateNow.durationFrom(lastCall);
	float dcConstrained = target - current;
	BoundAbs(dcConstrained, dcurrent * dt);
	float newCurrent = current + dcConstrained;

	lastCall = dateNow;
	current = newCurrent;

	return current;
}




Smoother::~Smoother() {
}

