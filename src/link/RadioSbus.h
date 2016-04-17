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
	uint8_t sbusData[25];
	int16_t channelsCalib[18];
	int16_t channels[18];
	int16_t servos[18];
	uint8_t  failsafe_status;
	int sbus_passthrough;
	int toChannels;


	RadioSbus();
	void begin(void);
	int16_t Channel(uint8_t ch);
	uint8_t DigiChannel(uint8_t ch);
	void Servo(uint8_t ch, int16_t position);
	void DigiServo(uint8_t ch, uint8_t position);
	uint8_t Failsafe(void);
	void PassthroughSet(int mode);
	int PassthroughRet(void);
	void UpdateChannels(void);
	void FeedLine(void);

	bool isComLost() {
		return (Date::now().durationFrom(lastUpdate) > 1.0);
	}

	float getChannelNormed(uint8_t ch) {
		int16_t channelValue = RADIO_OFFSET;

		if (!isComLost()) {
			channelValue = Channel(ch);
		}
		return (channelValue - RADIO_OFFSET) / RADIO_VAR;
	}

private:
	uint8_t byte_in_sbus;
	uint8_t bit_in_sbus;
	uint8_t ch;
	uint8_t bit_in_channel;
	uint8_t bit_in_servo;
	uint8_t inBuffer[25];
	int bufferIndex;
	uint8_t inData;
	int feedState;

};

#endif /* LINK_RADIOSBUS_H_ */
