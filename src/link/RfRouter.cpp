/*
 * RfRouter.cpp
 *
 *  Created on: Oct 16, 2015
 *      Author: adrien
 */

#include <cstring>
#include "../data/conf/Conf.h"
#include "RfRouter.h"

RfRouter::RfRouter(RfControler *pControler)
{
	// Rf router check for new message at 20Hz and route packet regarding their header
	freqHz = 20;
	_controler = pControler;
}

void RfRouter::process()
{
	// If controler has new packet awaiting
	if (_controler->hasPacketPending())
	{
		// Remove first packet
		RfPacket packet = _controler->popFirstPacket();

		// Get header and payload
		std::string packetHeader = packet.getHeader();
		std::string packetStr = packet.getPayload();

		// Update a conf parameter
		if (packetHeader.compare("CONF") == 0)
		{
			Conf::getInstance().parseRf(packet.getPayload());
		}
		// Request configuration parameters
		else if (packetHeader.compare("CONF_REQUEST") == 0)
		{
			Conf::getInstance().sendConfToGcs();
		}
		else {
			// Unknow message
		}
	}
}
