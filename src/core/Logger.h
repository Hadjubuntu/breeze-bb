/*
 * Logger.h
 *
 *  Created on: Sep 24, 2015
 *      Author: adrien
 */

#ifndef CORE_LOGGER_H_
#define CORE_LOGGER_H_

#include <string>
#include "History.h"

enum LoggerMode
{
	DEBUG,
	INFO,
	OPERATIONNAL
};


class Logger {
private:
	LoggerMode _mode;
	std::string logFilepath;
	History<std::string> history;
public:
	Logger();

	void info(const char str[]) ;
	void debug(const char str[]);
	void error(const char str[]);

	// Save in local file
	void log(std::string);
	void appendLogfile();


	/**
	 * Setters
	 */
	void setLogFilepath(std::string pFilepath)
	{
		logFilepath = pFilepath;
	}

	/**
	 * Static functions
	 */
	static Logger getDefault() {
		Logger e;
		return e;
	}
};

#endif /* CORE_LOGGER_H_ */
