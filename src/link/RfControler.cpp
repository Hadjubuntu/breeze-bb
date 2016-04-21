/*
 * RfControler.cpp
 *
 *  Created on: Sep 15, 2015
 *      Author: adrien
 */


#include <stdio.h>
#include "RfControler.h"
#include "../core/StrUtils.h"

RfControler::RfControler() : Processing()
{
	freqHz = 50;
	_iterSendPacket = 0;
}

void RfControler::receiveNewPackets() {
	//FIXME
//	if (RfSerial.available() > 0) {
//		char inChar = (char) (RfSerial.read());
//		// If incoming character is a new line, then the packet is stored
//		//----------------------------------------------------------
//		if (inChar == _endPacketChar)
//		{
//			vector<string> datas = StrUtils::explode(_incomingPacket, '|');
//
//			if (datas.size() >= 1)
//			{
//				string payload = "";
//				if (datas.size() > 1) {
//					payload = datas[1];
//				}
//				// Add packet to history
//				RfPacket e(Date::now(), datas[0], payload);
//				_receivedPackets.push_back(e);
//			}
//
//			// Reset incoming packet
//			_incomingPacket.clear();
//		} else        // Otherwise append new char to current incoming packet
//		{
//			// Push back character to string
//			_incomingPacket += inChar;
//		}
//	}
}

void RfControler::sendPackets()
{
	_iterSendPacket ++;
	if (_toSendPackets.size() > 0) /** && _iterSendPacket >= _freqHz / 5.0) */
	{
		RfPacket packet = _toSendPackets.front();
		_toSendPackets.pop_front();
		send(packet);

		_iterSendPacket = 0;
	}
}

void RfControler::process()
{
	receiveNewPackets();
	sendPackets();

	checkMaxPacketInStack();
}

void RfControler::checkMaxPacketInStack()
{
	// Prevent from having too much packet in stack data
	if (_toSendPackets.size() > 15) {
		_toSendPackets.clear();
		// Throw error : too much packet to send
	}
	if (_receivedPackets.size() > 15) {
		_receivedPackets.clear();
	}
}

void RfControler::send(RfPacket& packet)
{
	string header = packet.getHeader();
	string payload = packet.getPayload();

	string packetStr = header + "|" + payload;

	int byteBuffer = packetStr.length() + 5;

	if (byteBuffer <= RF_PACKET_MAX_LENGTH)
	{
		// Create byte to send
		char charArray[byteBuffer];

		// Concatenate data
		sprintf(charArray, "%s|%d", packetStr.c_str(), packet.getId());

		// Send data
		logger.info(charArray);
	}
}

RfPacket RfControler::popFirstPacket()
{
	RfPacket packet = _receivedPackets.front();
	_receivedPackets.pop_front();
	return packet;
}

bool RfControler::hasPacketPending()
{
	return !_receivedPackets.empty();
}
