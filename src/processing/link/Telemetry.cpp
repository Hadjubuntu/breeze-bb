/*
 * Telemetry.cpp
 *
 *  Created on: Nov 8, 2015
 *      Author: adrien-i3
 */

#include <cstring>
#include <string>
#include <stdio.h>
#include "../../math/common/FastMath.h"
#include "Telemetry.h"
#include "../../link/RfPacket.h"


Logger telemetryLogger;

Telemetry::Telemetry(AHRS *pAhrs,
		FlightControl *pFlightControl,
		RfControler *pRfControler) : Processing()
{
	// Set pointer to attitude and heading reference processing
	_ahrs = pAhrs;
	_flightControl = pFlightControl;
	_rfControler = pRfControler;

	// 1 Hz updater
	freqHz = 1;
}

void Telemetry::init()
{

}

void Telemetry::process()
{
	// Build packet
	// --------------
	float rpy[3];
	_ahrs->getAttitude().toRollPitchYaw(rpy);
	char payload[RF_PACKET_MAX_LENGTH - 5];
	sprintf(payload, "%d;%d;%d;%d;%d;%d",
			FastMath::toCenti(rpy[0]), // roll
			FastMath::toCenti(rpy[1]), // pitch
			FastMath::toCenti(rpy[2]), // yaw
			FastMath::toCenti(_flightControl->getRollDesired()), // roll desired
			FastMath::toCenti(_flightControl->getPitchDesired()), // pitch desired
			FastMath::toCenti(_ahrs->getAltitudeMeters())); // altitude in meters

	// At each loop, build a telemetry packet and send it via Logger
	RfPacket packet(Date::now(), "TM", payload);

	// Send packet
	// --------------
	// Disable yet
	_rfControler->addPacketToSend(packet);
}


