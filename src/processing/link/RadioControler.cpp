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
	freqHz = 100;
}

/**
 * Initialize radio controller
 */
void RadioControler::init()
{
	// Start serial for sbus radio
	_handler.begin();

	// Wait for two seconds
//	wait(2.0);
//
//	// Set calibration values
//	for (int i = 0; i < 7; i ++) {
//		_handler.channelsCalib[i] = _handler.channels[i];
//	}
}

void RadioControler::process()
{
//	// Read bus data
//	_handler.FeedLine();
//
//	// If data are complete
//	if (_handler.toChannels == 1)
//	{
//		// Update channel data and last update date
//		_handler.UpdateChannels();
//		_handler.toChannels = 0;
//		_handler.lastUpdate = Date::now();
//	}
}
