/*
 * FsAutotuneCollect.cpp
 *
 *  Created on: Apr 9, 2016
 *      Author: adrien
 */

#include "FsAutotuneCollect.h"

FsAutotuneCollect::FsAutotuneCollect(PID *pPid) : Processing()
{
	freqHz = 50;
	score = 0.0;
	pid = pPid;
}

void FsAutotuneCollect::process()
{
	// Evaluate score
	float currentScore = pid->getError();

	// Average score
	score = 0.7 * score + 0.3 * currentScore;
}

FsAutotuneCollect::~FsAutotuneCollect()
{
	// Default destructor
}

