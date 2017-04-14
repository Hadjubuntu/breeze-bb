/*
 * Brain.h
 *------------------------------------------------------
 *  Created on: Sep 15, 2015
 *      Author: adrien
 */

#ifndef CORE_BRAIN_H_
#define CORE_BRAIN_H_

#include "History.h"
#include "Processing.h"
#include "../math/time/Date.h"
#include "../processing/ahrs/AHRS.h"

/**
 * The brain manage time by calling processing needed to be executed on the schedule
 */
class Brain {
private:
	// delay between tick in microseconds
	const static int _delayTickUs = 5;

	// Tick id counter
	long _tickId;

	// Reference date
	Date _referenceDate;

	// Processing needed to be executed
	std::vector<Processing*> _processings;

	void processingExecution(Processing* proc);

public:
	/**
	 * Constructor brain
	 */
	Brain();


	/** Initialize processings */
	void initProcessings();

	/** Add a new processing on the scheduler */
	void addProcessing(Processing *e) { _processings.push_back(e); }


	/** Loop */
	void loop();



	// Getters
	//--------------------
	long getTickId() const { return _tickId; }
};

#endif /* CORE_BRAIN_H_ */
