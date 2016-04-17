/*
 * RfPacket.cpp
 *
 *  Created on: Sep 15, 2015
 *      Author: adrien
 */

#include "RfPacket.h"

RfPacket::RfPacket(Date pDate, std::string pHeader, std::string pPayload) : _date(pDate)
{
	_header = pHeader;
	_payload = pPayload;
	_groupId = 0;
	_id = 0;
}

RfPacket::RfPacket(Date pDate, std::string pHeader, std::string pPayload, int pGroupId) : _date(pDate)
{
	_header = pHeader;
	_payload = pPayload;
	_groupId = pGroupId;
	_id = 0;
}

