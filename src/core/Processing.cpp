/*
 * Processing.cpp
 *
 *  Created on: Sep 21, 2015
 *      Author: adrien
 */

#include <stdio.h>
#include "Processing.h"

Processing::Processing() : logger(Logger()), freqHz(50), lastExecutionDate(Date::zero()), dt(0.0),
callbackTrigger(false), callbackStartDate(Date::now()), callbackDtUs(0l)
{

}


bool Processing::isReady() {
	// Retrieve now date
	Date now = Date::now();

	// Compute duration from last execution
	float durationLastExecutionSeconds = now.durationFrom(lastExecutionDate);

	// Compute delta time between two execution of the processing
	float dtExecExpected = 1.0/freqHz;

	// Returns yes if processing needs to be executed
	return durationLastExecutionSeconds >= dtExecExpected;
}

bool Processing::isCallbackReady() {
	Date now = Date::now();

	float durationFromCallbackTrigger = now.durationFrom(callbackStartDate);
	if (durationFromCallbackTrigger >= DateUtils::microToSeconds(callbackDtUs)) {
		return true;
	}
	else {
		return false;
	}
}


void Processing::wait(float pDtWait)
{
	Date currentTime = Date::now();

	while (Date::now().durationFrom(currentTime) < pDtWait)
	{
		//		printf("Duration %.4f\n", Date::now().durationFrom(currentTime));
		// Update data
		process();
	}

}
