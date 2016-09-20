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

	printf("Radio controller calibration is progress..\n");
	while (handler.firstValue == false)
	{
		handler.fastLoop();
	}
	printf("First value setted, waiting for change\n");

	while (handlerNoChange())
	{
		handler.fastLoop();
	}
	printf("Change ok, calibration in 2 seconds\n");
	wait(2.0);

	// Setup calibration
	for (int k = 0; k < NB_CHANNELS_OPERATIONNAL;  k ++)
	{
		handler.channelsCalib[k] = handler.channels[k];
	}
	printf("Radio controller calibration done !\n");

}

bool RadioControler::handlerNoChange()
{
	bool hasChange = false;

	for (int k=0; k < NB_CHANNELS_OPERATIONNAL; k ++)
	{
		if (handler.channelsCalib[k] != handler.channels[k])
		{
			hasChange = true;
			break;
		}
	}

	return hasChange;
}

void RadioControler::process()
{
	handler.fastLoop();
}
