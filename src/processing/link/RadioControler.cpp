/*
 * RadioControler.cpp
 *
 *  Created on: Oct 18, 2015
 *      Author: adrien
 */

#include "RadioControler.h"

/**
 * Constructor
 */
RadioControler::RadioControler() : Processing()
{
	// 100 Hz updater
	freqHz = 1000;
}

/**
 * Initialize radio controller
 */
void RadioControler::init()
{
	// Start serial for sbus radio
	handler.init("/dev/ttyO1");

	// Wait for two seconds
	wait(2.0);

	// Set calibration values
	for (int i = 0; i < NB_CHANNELS_OPERATIONNAL; i ++)
	{
		handler.channelsCalib[i] = handler.channels[i];
	}
}

void RadioControler::process()
{
	handler.fastLoop();
}
