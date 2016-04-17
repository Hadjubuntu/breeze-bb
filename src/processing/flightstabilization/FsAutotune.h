/*
 * FsAutotune.h
 *
 *  Created on: Apr 9, 2016
 *      Author: adrien
 */

#ifndef PROCESSING_FLIGHTSTABILIZATION_FSAUTOTUNE_H_
#define PROCESSING_FLIGHTSTABILIZATION_FSAUTOTUNE_H_


#include "../../core/Processing.h"
#include "../../math/pid/PID.h"
#include "FsAutotuneStoreMeasure.h"
#include "FlightStabilization.h"
#include <vector>

class FsAutotune : public Processing {
protected:
	// Add a new PID to autotune
	void addAutotune(PID *pPid);
public:
	FsAutotune(FlightStabilization *flightStabilization);

	void init()
	{

	}

	/**
	 * Process and update data
	 */
	void process();
	void callback() {};


	void updateMeasure(PID);
	void autotune(PID);


	float getFirstMeasureScore() {
		float scoreOutput = 999.0;

		if (getProcChildren().size() > 0) {
			FsAutotuneStoreMeasure *storeMeasure = ((FsAutotuneStoreMeasure*) (getProcChildren().at(0)));

			scoreOutput = storeMeasure->getStoredScore();
		}

		return scoreOutput;
	}

	virtual ~FsAutotune();
};

#endif /* PROCESSING_FLIGHTSTABILIZATION_FSAUTOTUNE_H_ */
