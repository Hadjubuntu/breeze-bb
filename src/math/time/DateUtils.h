/*
 * DateUtils.h
 *
 *  Created on: Sep 21, 2015
 *      Author: adrien
 */

#ifndef MATH_TIME_DATEUTILS_H_
#define MATH_TIME_DATEUTILS_H_

#include "Date.h"


/**
 * Useful function to manipulate dates
 */
class DateUtils {
private:
	static constexpr float S_TO_US = 1000000.0f;
public:
	/**
	 * Transform delta time in microseconds into delta time in seconds
	 */
	static float microToSeconds(long dt) {
		return ((float) dt / S_TO_US);
	}
	static long secondsToMicros(float pSeconds) {
		return (long) (pSeconds * S_TO_US);
	}

	static uint32 micros() {
		return micros();
	}


};



#endif /* MATH_TIME_DATEUTILS_H_ */
