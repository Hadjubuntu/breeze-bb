/**
 * breeze-arm
 * Configuration.cpp
 * ----------------------------
 *
 * ----------------------------
 *  Created on: Oct 24, 2015
 *      Author: Adrien HADJ-SALAH
 */

#include <string>
#include <stdlib.h>
#include "Conf.h"
#include "../../core/StrUtils.h"

Conf Conf::INSTANCE = Conf();

/**
 * const float maxAbsRollAngle = 0.34; // 0.34 for 20 degress, 0.5 for approx 30 degrees, go higher to 0.8 for 45 degrees
	const float maxAbsPitchAngle = 0.34;
	const float maxAbsCombinedAngle = 0.5;
	// Max command on the torque in Nm
	const float maxCommandNm = 6.0;
	// Use boost motor to compensate roll/pitch angle
	const bool useBoostMotors = true;
 */
Conf::Conf()
{
	// 0.34 for 20 degress, 0.5 for approx 30 degrees, go higher to 0.8 for 45 degrees
	_parameters.push_back(Param<float>("maxAbsRollAngle", 1.0f));
	_parameters.push_back(Param<float>("maxAbsPitchAngle", 1.0f));
	_parameters.push_back(Param<float>("maxAbsCombinedAngle", 1.0f));
	// Max command on the torque in Nm
	_parameters.push_back(Param<float>("maxCommandNm", 6.0f));
	// Flight stabilization
	_parameters.push_back(Param<float>("flightStabilization_Pq", 15.0f));
	_parameters.push_back(Param<float>("flightStabilization_Pw", 1.0f));
	_parameters.push_back(Param<float>("commandNmToSignalUs", 80.0f));

	// Flight stabilization with simple PID
	_parameters.push_back(Param<float>("flightStabilization_Kangle", 4.0f));
	_parameters.push_back(Param<float>("flightStabilization_KrateRoll", 0.8f));
	_parameters.push_back(Param<float>("flightStabilization_KratePitch", 0.8f));
	_parameters.push_back(Param<float>("flightStabilization_KrateYaw", 4.0f));

	// Flight stab altitude controller
	_parameters.push_back(Param<float>("flightStabilization_throttleHover", 0.45f));

}



Param<float>* Conf::get(std::string pName)
{
	Param<float>* e;
	int idx = find(pName);

	if (idx >= 0)
	{
		e = &_parameters[idx];
	}
	else {
		// Unknow param
		e = &_parameters[0];
	}

	return e;
}

int Conf::set(std::string pName, float pValue)
{
	int idx = find(pName);
	if (idx >= 0)
	{
		_parameters[idx].setValue(pValue);
	}
	else
	{
		_parameters.push_back(Param<float>(pName, pValue));
	}

	return idx;
}

int Conf::find(std::string pName)
{
	int findIndex = -1;
	int index = 0;

	while (index < _parameters.size() && findIndex == -1)
	{
		if (_parameters[index].getName().compare(pName) == 0)
		{
			findIndex = index;
		}
		index ++;
	}

	return findIndex;
}

Conf& Conf::getInstance()
{
	return INSTANCE;
}

/**
 * Retrieve parameter name and value from payload packet message
 */
void Conf::parseRf(std::string payload)
{
	vector<string> paramSplitData = StrUtils::explode(payload, ';');

	RfPacket packet(Date::now(), "LOG", "received_conf_param");
	_rfControler->addPacketToSend(packet);

	std::vector<string>::iterator itrDatas = paramSplitData.begin();
	while (itrDatas != paramSplitData.end())
	{
		std::string paramName = *itrDatas;

		// Go to next parameter
		itrDatas ++;
		if (itrDatas != paramSplitData.end()) {

			std::string paramValueStr = *itrDatas;
			float paramValue = atof(paramValueStr.c_str());

			// If parameter exists, then set value
			if (find(paramName) > 0)
			{
				set(paramName, paramValue);
			}
			// Otherwise send to GCS the exception
			else
			{
				RfPacket packet(Date::now(), "LOG", "error_reading_conf");
				_rfControler->addPacketToSend(packet);
			}

			itrDatas ++;
		}
	}

}

void Conf::sendConfToGcs()
{
	for (Param<float> param : _parameters)
	{
		RfPacket packet(Date::now(), "CONF", param.toString());
		_rfControler->addPacketToSend(packet);
	}
}
