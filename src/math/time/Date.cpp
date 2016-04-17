/*
 * Date.cpp
 *
 *  Created on: Sep 15, 2015
 *      Author: adrien
 */

#include "Date.h"



int Date::compareTo(Date& date)
{
	if (_timeElapsedSinceStartUs < date.getTimeElapsedSinceStartUs()) {
		return -1;
	}
	else if (_timeElapsedSinceStartUs == date.getTimeElapsedSinceStartUs()) {
		return 0;
	}
	else {
		return 1;
	}
}

float Date::durationFrom(Date& date)
{
	return DateUtils::microToSeconds(_timeElapsedSinceStartUs - date.getTimeElapsedSinceStartUs());
}


Date Date::now()
{
	Date e(micros());
	return e;
}
