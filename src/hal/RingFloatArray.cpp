/*
 * RingFloatArray.cpp
 *
 *  Created on: Oct 16, 2016
 *      Author: adrien
 */

#include "../math/common/FastMath.h"
#include "RingFloatArray.h"

RingFloatArray::RingFloatArray(int pSize)
{
	size = pSize;
	for (int i=0; i < size; i ++)
	{
		values[i] = 0.0f;
	}
	posIdx = 0;
}

void RingFloatArray::add(float pElement)
{
	values[posIdx] = pElement;
	posIdx ++;
}

float RingFloatArray::integralValue()
{
	if (size > 0)
	{
		float integral = 0.0;

		for (int i=0; i < size; i ++)
		{
			integral += values[i];
		}

		return integral / size;
	}
	else {
		return 0.0;
	}
}

float RingFloatArray::absIntegralValue()
{
	if (size > 0)
	{
		float integral = 0.0;

		for (int i=0; i < size; i ++)
		{
			integral += FastMath::fabs(values[i]);
		}

		return integral / size;
	}
	else {
		return 0.0;
	}
}
