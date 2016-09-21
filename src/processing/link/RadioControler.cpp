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
	// Wait until a first value is setted
	while (handler.sbus_updated == false) {
		handler.fastLoop();
		iter ++;
	}
	// Then wait until throttle is down
	while (handler.channels[2] > RADIO_OFFSET + 100)
	{
		wait(0.5);
	}
	printf("Throttle down ok\n");

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
