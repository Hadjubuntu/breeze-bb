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
	// We want to warn user if a processing have been waiting for a duration equals to its delta calling time
	maxAwaitingProcessingSeconds = 1.0/freqHz;
}


bool Processing::isReady() {
	// Retrieve now date
	Date now = Date::now();

	// Compute duration from last execution
	float durationLastExecutionSeconds = now.durationFrom(lastExecutionDate);

	// Compute delta time between two execution of the processing
	float dtExecExpected = 1.0/freqHz;

	float diffDurationWaited = durationLastExecutionSeconds - dtExecExpected;

	if (diffDurationWaited > maxAwaitingProcessingSeconds)
	{
		printf("[WARNING] Processing waited for %.2f seconds while max awaiting is %.2f\n", diffDurationWaited, maxAwaitingProcessingSeconds);
	}

	// Returns yes if processing needs to be executed
	return diffDurationWaited >= 0;
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
