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

	waitUntilFullYawBackAndForth();

	// Setup calibration
	for (int k = 0; k < NB_CHANNELS_OPERATIONNAL;  k ++)
	{
		handler.channelsCalib[k] = handler.channels[k];
	}
}

void RadioControler::waitUntilFullYawBackAndForth()
{
	bool isDone = false, backDone = false;
	int firstValueYaw = handler.channels[3];
	Date initYawBack = Date::zero();


	printf("Waiting yaw back and forth operation to activate UAV\n");


	while (isDone == false)
	{
		printf("delta : %d\n", abs(handler.channels[3] - firstValueYaw) );
		if (abs(handler.channels[3] - firstValueYaw) > 200)
		{
			initYawBack = Date::now();
			backDone = true;
			printf("Delta ok\n");
		}

		if (backDone && Date::now().durationFrom(initYawBack) > 1.0)
		{
			if (abs(handler.channels[3] - firstValueYaw) < 30)
			{
				isDone = true;
			}
		}

		handler.fastLoop();
	}

	printf("Yaw is back and forth, initialization done\n");
}


void RadioControler::process()
{
	handler.fastLoop();
}
