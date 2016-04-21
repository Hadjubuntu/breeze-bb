/*
 * RadioSbus.cpp
 *
 *  Created on: Oct 17, 2015
 *      Author: adrien
 */

#include <cstring>
#include "../hal/HAL.h"
#include "RadioSbus.h"


/**
 * Default constructor
 */
RadioSbus::RadioSbus() : lastUpdate(Date::now())
{

}

void RadioSbus::begin(){
	short loc_sbusData[25] = {
			0x0f,0x01,0x04,0x20,0x00,0xff,0x07,0x40,0x00,0x02,0x10,0x80,0x2c,0x64,0x21,0x0b,0x59,0x08,0x40,0x00,0x02,0x10,0x80,0x00,0x00};
	int loc_channels[18]  = {
			1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,0,0};
	int loc_servos[18]    = {
			1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,0,0};
//	FIXME port.begin(BAUDRATE);

	memcpy(sbusData,loc_sbusData,25);
	memcpy(channels,loc_channels,18);
	memcpy(channelsCalib, loc_channels, 18);
	memcpy(servos,loc_servos,18);
	failsafe_status = SBUS_SIGNAL_OK;
	sbus_passthrough = 1;
	toChannels = 0;
	bufferIndex = 0;
	feedState = 0;
	lastUpdate = Date::now();

	for (int i = 0; i < 18; i ++) {
		channels[i] = 0;
	}
}

int RadioSbus::Channel(short ch) {
	// Read channel data
	if ((ch>0)&&(ch<=16)){
		return channels[ch-1];
	}
	else{
		return 1023;
	}
}
short RadioSbus::DigiChannel(short ch) {
	// Read digital channel data
	if ((ch>0) && (ch<=2)) {
		return channels[15+ch];
	}
	else{
		return 0;
	}
}
void RadioSbus::Servo(short ch, int position) {
	// Set servo position
	if ((ch>0)&&(ch<=16)) {
		if (position>2048) {
			position=2048;
		}
		servos[ch-1] = position;
	}
}
void RadioSbus::DigiServo(short ch, short position) {
	// Set digital servo position
	if ((ch>0) && (ch<=2)) {
		if (position>1) {
			position=1;
		}
		servos[15+ch] = position;
	}
}
short RadioSbus::Failsafe(void) {
	return failsafe_status;
}

void RadioSbus::PassthroughSet(int mode) {
	// Set passtrough mode, if true, received channel data is send to servos
	sbus_passthrough = mode;
}

int RadioSbus::PassthroughRet(void) {
	// Return current passthrough mode
	return sbus_passthrough;
}

void RadioSbus::UpdateChannels(void)
{
	channels[0]  = ((sbusData[1]|sbusData[2]<< 8) & 0x07FF);
	channels[1]  = ((sbusData[2]>>3|sbusData[3]<<5) & 0x07FF);
	channels[2]  = ((sbusData[3]>>6|sbusData[4]<<2|sbusData[5]<<10) & 0x07FF);
	channels[3]  = ((sbusData[5]>>1|sbusData[6]<<7) & 0x07FF);
	channels[4]  = ((sbusData[6]>>4|sbusData[7]<<4) & 0x07FF);
	channels[5]  = ((sbusData[7]>>7|sbusData[8]<<1|sbusData[9]<<9) & 0x07FF);
	channels[6]  = ((sbusData[9]>>2|sbusData[10]<<6) & 0x07FF);
	channels[7]  = ((sbusData[10]>>5|sbusData[11]<<3) & 0x07FF);
	// & the other 8 + 2 channels if you need them
#ifdef ALL_CHANNELS
	channels[8]  = ((sbusData[12]|sbusData[13]<< 8) & 0x07FF);
	channels[9]  = ((sbusData[13]>>3|sbusData[14]<<5) & 0x07FF);
	channels[10] = ((sbusData[14]>>6|sbusData[15]<<2|sbusData[16]<<10) & 0x07FF);
	channels[11] = ((sbusData[16]>>1|sbusData[17]<<7) & 0x07FF);
	channels[12] = ((sbusData[17]>>4|sbusData[18]<<4) & 0x07FF);
	channels[13] = ((sbusData[18]>>7|sbusData[19]<<1|sbusData[20]<<9) & 0x07FF);
	channels[14] = ((sbusData[20]>>2|sbusData[21]<<6) & 0x07FF);
	channels[15] = ((sbusData[21]>>5|sbusData[22]<<3) & 0x07FF);
#endif
	// DigiChannel 1
	/*if (sbusData[23] & (1<<0)) {
    channels[16] = 1;
  }
  else{
    channels[16] = 0;
  }
  // DigiChannel 2
  if (sbusData[23] & (1<<1)) {
    channels[17] = 1;
  }
  else{
    channels[17] = 0;
  }*/
	// Failsafe
	failsafe_status = SBUS_SIGNAL_OK;
	if (sbusData[23] & (1<<2)) {
		failsafe_status = SBUS_SIGNAL_LOST;
	}
	if (sbusData[23] & (1<<3)) {
		failsafe_status = SBUS_SIGNAL_FAILSAFE;
	}

}
void RadioSbus::FeedLine(void){
	//FIXME
//	if (port.available() > 24) {
//		while(port.available() > 0) {
//			inData = port.read();
//
//
//			switch (feedState){
//			case 0:
//				if (inData != 0x0f){
//					while(port.available() > 0){//read the contents of in buffer this should resync the transmission
//						inData = port.read();
//					}
//					return;
//				}
//				else{
//					bufferIndex = 0;
//					inBuffer[bufferIndex] = inData;
//					inBuffer[24] = 0xff;
//					feedState = 1;
//				}
//				break;
//			case 1:
//				bufferIndex ++;
//				inBuffer[bufferIndex] = inData;
//				if (bufferIndex < 24 && port.available() == 0) {
//					feedState = 0;
//				}
//				if (bufferIndex == 24) {
//					feedState = 0;
//					if (inBuffer[0]==0x0f && inBuffer[24] == 0x00) {
//						// Copy data
//						memcpy(sbusData,inBuffer,25);
//						toChannels = 1;
//					}
//				}
//				break;
//			}
//		}
//	}
}

