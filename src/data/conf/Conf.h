/**
 * breeze-arm
 * Configuration.h
 * ----------------------------
 *
 * ----------------------------
 *  Created on: Oct 24, 2015
 *      Author: Adrien HADJ-SALAH
 */

#ifndef DATA_CONF_CONF_H_
#define DATA_CONF_CONF_H_

#include <vector>
#include "../../link/RfControler.h"
#include "Param.h"

enum Firmware: int {
	FIXED_WING = 0,
			HCOPTER = 1,
			YCOPTER = 2,
};

class Conf {
private:
	// Singleton variables and functions
	// ------------------------------------
	static Conf INSTANCE;

	Conf& operator= (const Conf&){}
	Conf (const Conf &){}

	Conf() ;

	std::vector<Param<float>> _parameters;
	RfControler *_rfControler;

public:
	static Conf& getInstance();

	/**
	 * Get parameter value from its name
	 */
	Param<float>* get(std::string pName);

	/**
	 * Set param name to value and returns index
	 * if parameter was already existing in the parameters list
	 */
	int set(std::string pName, float pValue);

	/**
	 * Find parameter index by its name in the list
	 */
	int find(std::string pName);


	// Default configuration parameters
	// ------------------------------------
	// Use boost motor to compensate roll/pitch angle
	const bool useBoostMotors = false;


	// Firmware
	const int firmware = Firmware::FIXED_WING;


	/**
	 * Parse rf payload message and update configuration
	 */
	void parseRf(std::string payload);

	/**
	 * Send configuration to ground control station
	 */
	void sendConfToGcs();

	void setRfControler(RfControler *pControler)
	{
		_rfControler = pControler;
	}

	std::string firmwareToString()
	{
		if (firmware == Firmware::HCOPTER)
		{
			return std::string("hcopter");
		}
		else if (firmware == Firmware::YCOPTER)
		{
			return std::string("ycopter");
		}
		else if (firmware == Firmware::FIXED_WING)
		{
			return std::string("fixed_wing");
		}

		return std::string("hcopter"); // default
	}
};

#endif /* DATA_CONF_CONF_H_ */
