/*
 * RadioControler.cpp
 *
 *  Created on: Oct 18, 2015
 *      Author: adrien
 */

#include "RadioControler.h"

#define CH_ROLL 0
#define CH_PITCH 1
#define CH_THROTTLE 2
#define CH_YAW 3
#define CH_ROLL_MIX 6 // Fixed-wing only

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
	// Execute screen temporary command
	// (this has been added to fix a bug where sbus is not read until screen is launched. Screen
	// command is then killed to limit CPU usage)
	std::string cmdScreen = "sudo screen -dm /dev/ttyO1 57600 && sudo killall screen";
	int resCmd = system(cmdScreen.c_str());
	printf("bb screen result: %d\n", resCmd);

	// Start serial for sbus radio
	handler.init("/dev/ttyO1");


	int iter = 0;
	printf("RadioControler is waiting for throttle down..\n");

	printf("Entering fast loop\n");
	// Wait until a first value is setted
	while (handler.sbus_updated == false) {
		handler.fastLoop();
		iter ++;
	}
	// Then wait until throttle is down
	while (handler.channels[2] > RADIO_OFFSET + 100)
	{
		printf("Throttle still at: %d\n", handler.channels[2]);
		wait(1.0);
	}
	printf("Throttle down ok\n");

	// Setup calibration
	for (int k = 0; k < NB_CHANNELS_OPERATIONNAL;  k ++)
	{
		handler.channelsCalib[k] = handler.channels[k];
	}
}


// ---------------------------------------------
// Normed commands
// ---------------------------------------------

float RadioControler::getRollCommandNormed()
{
	return handler.getChannelNormed(CH_ROLL);
}

float RadioControler::getPitchCommandNormed()
{
	return handler.getChannelNormed(CH_PITCH);
}

float RadioControler::getThrottleCommandNormed()
{
	return handler.getChannelNormed(CH_THROTTLE);
}

float RadioControler::getYawCommandNormed()
{
	return handler.getChannelNormed(CH_YAW);
}

// ---------------------------------------------
// Raw commands
// ---------------------------------------------
int RadioControler::getRollRawCommand()
{
	return handler.getChannel(CH_ROLL);
}

int RadioControler::getRollMixRawCommand()
{
	return handler.getChannel(CH_ROLL_MIX);
}

int RadioControler::getPitchRawCommand()
{
	return handler.getChannelNormed(CH_PITCH);
}

int RadioControler::getThrottleRawCommand()
{
	return handler.getChannel(CH_THROTTLE);
}

int RadioControler::getYawRawCommand()
{
	return handler.getChannelNormed(CH_YAW);
}

// Process in fast loop mode
void RadioControler::process()
{
	handler.fastLoop();
}
