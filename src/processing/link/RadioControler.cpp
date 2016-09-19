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

	printf("Radio controller calibration is progrss..\n");
	while (handler.channels[2] == RADIO_OFFSET || handler.channels[2] > RADIO_OFFSET  + 100)
	{
		handler.fastLoop();
	}

	// Setup calibration
	for (int k = 0; k < NB_CHANNELS_OPERATIONNAL;  k ++)
	{
		handler.channelsCalib[k] = handler.channels[k];
	}
	printf("Radio controller calibration done !\n");

}

void RadioControler::process()
{
	handler.fastLoop();
}
