/*
 * Processing.h
 *
 *  Created on: Sep 21, 2015
 *      Author: adrien
 */

#ifndef CORE_PROCESSING_H_
#define CORE_PROCESSING_H_

#include <vector>
#include "../core/Logger.h"
#include "../math/time/Date.h"


/**
 * Abstract class of a processing
 */
class Processing {
protected:
	/** Internal logger */
	Logger logger;

	/** Frequency of process function calling (Hertz) */
	int freqHz;

	/** Date last execution */
	Date lastExecutionDate;


	/** Last dt (Seconds) */
	float dt;

	/** Callback is called */
	bool callbackTrigger;
	Date callbackStartDate;
	long callbackDtUs;

	/** Sub-processings */
	std::vector<Processing*> procChildren;

public:
	Processing();

	/** Initialize processing */
	virtual void init() = 0;

	/** Execute processing */
	virtual void process() = 0;

	virtual void callback() = 0;


	/**
	 * Update execution date
	 * This function is called by UAV brain
	 */
	void updateExecDate()
	{
		// Get now date
		Date now = Date::now();

		// Store dt
		dt = now.durationFrom(lastExecutionDate);

		// Update last execution date
		lastExecutionDate = now;
	}

	/** Wait for dt seconds */
	void wait(float dtWait);

	/**
	 * Tells whether the processing needs to be called again or not
	 */
	bool isReady();
	bool isCallbackReady();

	bool isTriggeringCallback() {
		return callbackTrigger;
	}
	void closeCallback() {
		callbackTrigger = false;
		callbackDtUs = 0;
	}
	void planCallback(long pDtUs) {
		callbackDtUs = pDtUs;
		callbackTrigger = true;
		callbackStartDate = Date::now();
	}

	std::vector<Processing*> getProcChildren() {
		return procChildren;
	}

	/** Add a processing child */
	void addProcChild(Processing *child) {
		procChildren.push_back(child);
	}
};

#endif /* CORE_PROCESSING_H_ */
