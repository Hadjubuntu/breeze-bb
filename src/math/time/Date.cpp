/*
 * Date.cpp
 *
 *  Created on: Sep 15, 2015
 *      Author: adrien
 */
#include <chrono>
#include <stdio.h>
#include "Date.h"


std::chrono::time_point<std::chrono::system_clock> dateOrigin = std::chrono::system_clock::now();

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


long Date::micros()
{
	std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();

	long micros = std::chrono::duration_cast<std::chrono::microseconds>
	(now-dateOrigin).count();

	return micros;
}

Date Date::now()
{
	Date e(micros());
	return e;
}
