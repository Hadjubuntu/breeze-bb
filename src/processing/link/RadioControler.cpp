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


	int iter = 0;
	printf("waiting for throttle down\n");
	while (iter < 500) {
		handler.fastLoop();
		iter ++;
	}
	iter = 0;
	while (iter < 1000  ||  (handler.channels[3] > (RADIO_OFFSET + RADIO_VAR / 10.0)))
	{
		handler.fastLoop();
		iter ++;
	}
	wait(1.0);
	printf("throttle done ok => calibration\n");

	// Setup calibration
	for (int k = 0; k < NB_CHANNELS_OPERATIONNAL;  k ++)
	{
		handler.channelsCalib[k] = handler.channels[k];
	}
}

float RadioControler::getRollCommandNormed()
{
	return handler.getChannelNormed(0);
}

float RadioControler::getPitchCommandNormed()
{
	return handler.getChannelNormed(1);
}

float RadioControler::getThrottleCommandNormed()
{
	return handler.getChannelNormed(2);
}

float RadioControler::getYawCommandNormed()
{
	return handler.getChannelNormed(3);
}

void RadioControler::process()
{
	handler.fastLoop();
}
