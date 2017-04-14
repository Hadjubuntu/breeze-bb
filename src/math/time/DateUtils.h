/*
 * DateUtils.h
 *
 *  Created on: Sep 21, 2015
 *      Author: adrien
 */

#ifndef MATH_TIME_DATEUTILS_H_
#define MATH_TIME_DATEUTILS_H_

#include "Date.h"
#include "../../core/Constants.h"


/**
 * Useful function to manipulate dates
 */
class DateUtils {
public:
	/**
	 * Transform delta time in microseconds into delta time in seconds
	 */
	static float microToSeconds(long dt) {
		return ((float) dt / Constants::S_TO_US);
	}
	static long secondsToMicros(float pSeconds) {
		return (long) (pSeconds * Constants::S_TO_US);
	}
	static long microToNanos(float pDtUs) {
		return (long)(pDtUs * Constants::US_TO_NS);
	}
	static int micros() {
		return micros();
	}


};



#endif /* MATH_TIME_DATEUTILS_H_ */
