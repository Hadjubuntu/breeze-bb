/*
 * RadioSbus.h
 *
 *  Created on: Oct 17, 2015
 *      Author: adrien
 */

#ifndef LINK_RADIOSBUS_H_
#define LINK_RADIOSBUS_H_

#include <cstdint>
#include "../math/time/Date.h"

#define SBUS_SIGNAL_OK          0x00
#define SBUS_SIGNAL_LOST        0x01
#define SBUS_SIGNAL_FAILSAFE    0x03
#define SBUS_AUTO_CHANNEL 		4
#define BAUDRATE 100000
#define SBUS_SIGNAL_LOST_DELAY_US 800000L
#define RADIO_OFFSET 1023.0f
#define RADIO_VAR 680.0f
//#define ALL_CHANNELS


/**
 * Memo channels:
 * ch0 : roll
 * ch1 : pitch
 * ch2: throttle
 * ch3: yaw
 * ch... : aux
 */
class RadioSbus {
public:
	Date lastUpdate;
	short sbusData[25];
	int channelsCalib[18];
	int channels[18];
	int servos[18];
	short  failsafe_status;
	int sbus_passthrough;
	int toChannels;


	RadioSbus();
	void begin(void);
	int Channel(short ch);
	short DigiChannel(short ch);
	void Servo(short ch, int position);
	void DigiServo(short ch, short position);
	short Failsafe(void);
	void PassthroughSet(int mode);
	int PassthroughRet(void);
	void UpdateChannels(void);
	void FeedLine(void);

	bool isComLost() {
		return (Date::now().durationFrom(lastUpdate) > 1.0);
	}

	float getChannelNormed(short ch) {
		int channelValue = RADIO_OFFSET;

		if (!isComLost()) {
			channelValue = Channel(ch);
		}
		return (channelValue - RADIO_OFFSET) / RADIO_VAR;
	}

private:
	short byte_in_sbus;
	short bit_in_sbus;
	short ch;
	short bit_in_channel;
	short bit_in_servo;
	short inBuffer[25];
	int bufferIndex;
	short inData;
	int feedState;

};

#endif /* LINK_RADIOSBUS_H_ */
