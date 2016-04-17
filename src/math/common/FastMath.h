/*
 * FastMath.h
 *
 *  Created on: Sep 18, 2015
 *      Author: adrien
 */

#ifndef MATH_COMMON_FASTMATH_H_
#define MATH_COMMON_FASTMATH_H_

#define min(a,b) ((a)<(b)?(a):(b))
//#define max(a,b) ((a)>(b)?(a):(b))
#define Bound(_x, _min, _max) { if (_x > (_max)) _x = (_max); else if (_x < (_min)) _x = (_min); }
#define BoundAbs(_x, _max) Bound(_x, -(_max), (_max))


class FastMath {
private:
	static constexpr float RAD2DEG = 57.29577951f;
public:

	FastMath();

	// Scalar functions
	// ---
	static float toDegrees(float angleRadians);
	static float toRadians(float angleDegrees);
	static float exp(float x);
	static float fastpow2(float x);
	static float sqrt(float x);
	static float fast_invsqrtf(float);
	static int toCenti(float x);
	static float constrainAngleZeroToTwoPi(float);
	static float constrainAngleMinusPiPlusPi(float);
	static float fabs(float);
};

#endif /* MATH_COMMON_FASTMATH_H_ */
