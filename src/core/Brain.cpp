/*
 * Brain.cpp

 *
 *  Created on: Sep 15, 2015
 *      Author: adrien
 */
#include <stdio.h>
#include "../core/Brain.h"
#include "../hal/I2C.h"
#include "../hal/HAL.h"


// SEE
//https://github.com/opendrone/flymaple-cpp/blob/master/libmaple/examples/test-timers.cpp


Brain::Brain() : _referenceDate(Date::zero()) {
	_tickId = 0;
	_referenceDate.setTimElapsedSinceStartUs(Date::micros());
}



void Brain::initProcessings()
{
	for (Processing *proc : _processings)
	{
		printf("Init proc ..\n");
		proc->init();
	}
}

void Brain::loop()
{
	// For each processing
	for (Processing *proc : _processings)
	{
		processingExecution(proc);
	}


	_tickId ++;
	if (_tickId > 1000000) {
		_tickId = 0;
	}
	HAL::delayUs(_delayTickUs);
}


void Brain::processingExecution(Processing* proc) {
	// Normal processing mode - check processing freq last call
	// ----
	if (proc->isReady() && !proc->isTriggeringCallback()) {
		// Update execution date
		proc->updateExecDate();
		// Then execute the processing
		proc->process();
	}
	// Callback triggering processing mode
	// ---
	if (proc->isTriggeringCallback() && proc->isCallbackReady()) {
		proc->callback();
		proc->closeCallback();
	}

	// Call iteratively processing execution function for children processings
	for (Processing *subProc : proc->getProcChildren())
	{
		processingExecution(subProc);
	}
}


