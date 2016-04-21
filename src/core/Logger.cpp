/*
 * Logger.cpp
 *
 *  Created on: Sep 24, 2015
 *      Author: adrien
 */

#include <string>
#include "Logger.h"

#include "../hal/HAL.h"


Logger::Logger()  : _mode(DEBUG) {
	// Initialize serial
//	Serial3.begin(57600);
}

void Logger::info(const char str[])
{
	if (_mode == INFO || _mode == DEBUG)
	{
//		Serial3.println(str);
	}
}

void Logger::debug(const char str[])
{
	if (_mode == DEBUG) {
//		Serial3.println(str);
	}
}

void Logger::error(const char str[]) {
//	Serial3.println(str);
}
