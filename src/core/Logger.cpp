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
		printf("[INFO] %s\n", str);
	}
}

void Logger::debug(const char str[])
{
	if (_mode == DEBUG) {
		printf("[DEBUG] %s\n", str);
	}
}

void Logger::error(const char str[]) {
	printf("[ERROR] %s\n", str);
}
