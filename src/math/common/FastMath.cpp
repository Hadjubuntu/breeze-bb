/*
 * FastMath.cpp
 *
 *  Created on: Sep 18, 2015
 *      Author: adrien
 */

#include <stdint.h>
#include <cmath>
#include "../../hal/HalMath.h"
#include "FastMath.h"



FastMath::FastMath() {

}
float FastMath::toRadians(float angleDegrees)
{
	return angleDegrees / RAD2DEG;
}

float FastMath::toDegrees(float angleRadians)
{
	return angleRadians * RAD2DEG;
}

float FastMath::fastpow2(float p)
{
	float offset = (p < 0) ? 1.0f : 0.0f;
	float clipp = (p < -126) ? -126.0f : p;
	int w = clipp;
	float z = clipp - w + offset;
	union { uint32_t i; float f; } v = { uint32_t ( (1 << 23) * (clipp + 121.2740575f + 27.7280233f / (4.84252568f - z) - 1.49012907f * z) ) };

	return v.f;
}

float FastMath::exp(float x)
{
	return FastMath::fastpow2(1.442695040f * x);
}

float FastMath::sqrt(float x) {
	return std::sqrt(x);
}


float FastMath::fast_invsqrtf(float number)   // rel. err. < 0.07%
{                               // Jan Kaldec, http://rrrola.wz.cz/inv_sqrt.html
	float x2, y;
	const float threehalfs = 1.5F;

	union {
		float    f;
		uint32_t u;
	} i;

	x2  = number * 0.5F;
	y   = number;

	i.f = y; // evil floating point bit level hacking
	i.u = 0x5f3759df - (i.u >> 1); // what the fxck?
	y   = i.f;
	y   = y * (threehalfs - (x2 * y * y));   // 1st iteration

	return y;
}


int FastMath::toCenti(float x)
{
	return (int) (100*x);
}

float FastMath::constrainAngleZeroToTwoPi(float x)
{
	float res = x;
	while (res > 2 * PI) {
		res -= 2 * PI;
	}
	while (res < 0.0) {
		res += 2 * PI;
	}

	return res;
}

float FastMath::constrainAngleMinusPiPlusPi(float x)
{
	float res = x;
	while (res >  PI) {
		res -= PI;
	}
	while (res < -PI) {
		res += PI;
	}

	return res;
}

float FastMath::fabs(float x)
{
	if (x >= 0.0) {
		return x;
	}
	else {
		return -x;
	}
}

float FastMath::pow(float a, float power) {
	return pow(a, power);
}

float FastMath::cos(float x)
{
	return cos(x);
}
