/*
 * Date.h
 *
 *  Created on: Sep 15, 2015
 *      Author: adrien
 */

#ifndef MATH_TIME_DATE_H_
#define MATH_TIME_DATE_H_


#include "../../hal/HAL.h"
#include "DateUtils.h"

class Date {
private:
	long _timeElapsedSinceStartUs;
public:
	/**
	 * Constructor
	 */
	Date(long timeElapsedSinceStartUs)  : _timeElapsedSinceStartUs(timeElapsedSinceStartUs) {
	}


	/**
	 * Compare two dates
	 */
	int compareTo(Date& date);

	/**
	 * Returns duration from another date in seconds
	 */
	float durationFrom(Date& date);


	/** *********************************************
	 * Getters
	 * ******************************************** */
	long getTimeElapsedSinceStartUs() {
		return _timeElapsedSinceStartUs;
	}

	/** *********************************************
	 * Setters
	 * ******************************************** */
	void setTimElapsedSinceStartUs(long tElpasedSinceStartUs) {
		_timeElapsedSinceStartUs = tElpasedSinceStartUs;
	}

	/** *********************************************
	 * Static functions
	 * ******************************************** */
	static Date zero() {
		Date e(0l);
		return e;
	}

	static Date now();

	static long micros();


};

#endif /* MATH_TIME_DATE_H_ */
