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
	iterCalibration = 0;
}

/**
 * Initialize radio controller
 */
void RadioControler::init()
{
	// Start serial for sbus radio
	handler.init("/dev/ttyO1");
}

void RadioControler::process()
{
	handler.fastLoop();

	if (iterCalibration == 99) {
		// Do nothing
	}
	else {
		if (iterCalibration < 100 && handler.isFailsafe() == false)
		{
			if (iterCalibration >= 90)
			{
				for (int k=0; k < NB_CHANNELS_OPERATIONNAL; k ++)
				{
					handler.channelsCalib[k] = handler.channels[k];
				}

				printf("Initialize done");
			}

			iterCalibration ++;
		}
	}

}
