/*
 * Logger.h
 *
 *  Created on: Sep 24, 2015
 *      Author: adrien
 */

#ifndef CORE_LOGGER_H_
#define CORE_LOGGER_H_

#include <string>

enum LoggerMode
{
	DEBUG,
	INFO,
	OPERATIONNAL
};


class Logger {
private:
	LoggerMode _mode;
public:
	Logger();

	void info(const char str[]) ;
	void debug(const char str[]);
	void error(const char str[]);

	static Logger getDefault() {
		Logger e;
		return e;
	}
};

#endif /* CORE_LOGGER_H_ */
