/*
 * Brain.cpp

 *
 *  Created on: Sep 15, 2015
 *      Author: adrien
 */
#include <stdio.h>
#include "../core/Brain.h"
#include "../hal/i2c.h"
#include "../hal/HAL.h"


// SEE
//https://github.com/opendrone/flymaple-cpp/blob/master/libmaple/examples/test-timers.cpp


Brain::Brain() : _referenceDate(Date::zero()), _commands(History<int>::getDefault()) {
	_tickId = 0;
//	FIXME _referenceDate.setTimElapsedSinceStartUs(micros());
}

void Brain::enableI2C()
{
	// Initialize I2C com
//	i2c_master_enable(I2C1, I2C_FAST_MODE);
}

void Brain::initProcessings()
{
	for (Processing *proc : _processings)
	{
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


Processing* Brain::getProcByName(std::string pName)
{
//	Processing *output;
//	std::vector<Processing>::iterator itrProc = _processings.begin();
//
//	while (itrProc != _processings.end() && output == NULL)
//	{
//		Processing current = *itrProc;
//		if (current.getName())
//		{
//
//		}
//		itrProc ++;
//	}
	return 0;
}

