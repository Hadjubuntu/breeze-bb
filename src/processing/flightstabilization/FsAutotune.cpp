/*
 * FsAutotune.cpp
 *
 *  Created on: Apr 9, 2016
 *      Author: adrien
 */

#include "FsAutotune.h"

FsAutotune::FsAutotune(FlightStabilization *flightStabilization) :  Processing()
{
	freqHz = 50;
	fs = flightStabilization;
	subprocessingAdded = false;
}

void FsAutotune::init()
{

}

/**
 * Add a new PID to optimize
 */
void FsAutotune::addAutotune(PID *pPid)
{
	FsAutotuneStoreMeasure storeMeasure(pPid);
	addProcChild(&storeMeasure);
}

void FsAutotune::process()
{
	if (!subprocessingAdded && fs != nullptr) 
	{
		PID pidRoll = fs->getPidRoll();
		// addAutotune(&pidRoll);
		subprocessingAdded = true;
	}

}

void FsAutotune::updateMeasure(PID pid)
{
	// Check stateDate, if dt > 1 second => change state (2 > 1, current measure > 2, restart current measure)
	// Notify autotune to start

	// BETTER IDEA TO implement => Easier if :
	// Processing 1hz which store and delete
	// while processing collect data ?
}

void FsAutotune::autotune(PID pid)
{
	// For a PID

	// If measure updated and not computed

	// score - prevScore (integral ?)
	// PID score = responsiveness ; error overhead ; mean error

	// If ds > 0 || probability to change
	// 		Accept parameter
	// Otherwise
	// 		Rollback

	// --------MEASURE1---------1s-----------MEASURE2---------2s--------....------>
}

FsAutotune::~FsAutotune()
{
	/** Default destructor */
}

