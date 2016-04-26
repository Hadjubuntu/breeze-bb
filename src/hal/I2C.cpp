/*
 * I2C.cpp
 *
 *  Created on: Sep 23, 2015
 *      Author: adrien
 */

#include "I2C.h"


// This is the instance of the libmaple I2C device to use
#define FLYMAPLE_I2C_DEVICE I2C1

struct i2c_msg {
	short addr;
	short flags;
	int length;
	short* data;
};

int I2C::writeTo(short addr, short val)
{
	//FIXME !!!
	return 0;
//	// all i2c transactions send and receive arrays of i2c_msg objects
//	i2c_msg msgs[1]; // we dont do any bursting here, so we only need one i2c_msg object
//	short msg_data[2] = {addr,val};
//
//	msgs[0].addr = _deviceAddr;
//	msgs[0].flags = 0; // write
//	msgs[0].length = 2; //Write two data
//	msgs[0].data = msg_data;
//	int res = i2c_master_xfer(I2C1, msgs, 1,0);  //
//
//	return res;
}

int I2C::readFrom(short addr, short nbBytes, short *msgData)
{
	/**i2c_msg msgs[2];

	msgData[0] = addr;

	msgs[0].addr = _deviceAddr;
	msgs[0].flags = 0; //write flag is 0
	msgs[0].length = 1; // just one byte for the address to read, 0x00
	msgs[0].data = msgData;

	msgs[1].addr = _deviceAddr;
	msgs[1].flags = I2C_MSG_READ; //read
	msgs[1].length = nbBytes; // Read the number of bytes
	msgs[1].data = msgData;
	int res = i2c_master_xfer(I2C1, msgs, 2, 0);
//	if (res != 0) {
//		reset();
//	}
	 */
// FIXME Manage I2C on beaglebone black
	return 0;

	//	i2c_msg msgs[1];
//	msgData[0] = addr;
//
//	msgs[0].addr = _deviceAddr;
//	msgs[0].flags = 0; //write flag is 0
//	msgs[0].length = 1; // just one byte for the address to read, 0x00
//	msgs[0].data = msgData;
//	i2c_master_xfer(I2C1, msgs, 1,0);
//
//	msgs[0].addr = _deviceAddr;
//	msgs[0].flags = I2C_MSG_READ; //read
//	msgs[0].length = nbBytes; // Read the number of bytes
//	msgs[0].data = msgData;
//	int res = i2c_master_xfer(I2C1, msgs, 1,0);
//
//	return res;
}

void I2C::reset()
{
	// FIXME
//	i2c_master_enable(I2C1, I2C_FAST_MODE);
}
