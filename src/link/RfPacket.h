/*
 * RfPacket.h
 *
 *  Created on: Sep 15, 2015
 *      Author: adrien
 */

#ifndef LINK_RFPACKET_H_
#define LINK_RFPACKET_H_

#include <string>
#include "../math/time/Date.h"

using namespace std;

#define RF_PACKET_MAX_LENGTH 95

class RfPacket {
private:
	int _id;
	Date _date;
	int _groupId; // 0 means no group id, otherwise unique id for the packets group
	std::string _header;
	std::string _payload;

public:
	/**
	 * Default constructor
	 * @param pDate date
	 * @param pHeader header
	 * @param pPayload Payload
	 */
	RfPacket(Date pDate, std::string pHeader, std::string pPayload);
	RfPacket(Date pDate, std::string pHeader, std::string pPayload, int pGroupId);


	/** ***********************************************
	 *  SETTER
	 ************************************************ */
	void setGroupId(int pGroupId)
	{
		_groupId = pGroupId;
	}

	/** ***********************************************
	 *  GETTERS
	 ************************************************ */
	std::string getHeader() {
		return _header;
	}
	std::string getPayload() {
		return _payload;
	}

	int length() {
		return _header.length() + _payload.length();
	}

	int getId() {
		return _id;
	}
};

#endif /* LINK_RFPACKET_H_ */
