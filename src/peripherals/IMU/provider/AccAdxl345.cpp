/*
 * AccAdxl345.cpp
 *
 *  Created on: May 10, 2016
 *      Author: adrien
 */

#include "AccAdxl345.h"

////////Acceleration sensor ADXL345 function/////////////////////////////
#define ACC (0x53)    // Defined ADXL345 address, ALT ADDRESS pin is grounded
#define A_TO_READ (6) // Number of bytes to read(each axis accounted for two-byte)
#define XL345_DEVID   0xE5 // ADXL345 ID register
// ADXL345 Control register
#define ADXLREG_TAP_AXES     0x2A
#define ADXLREG_BW_RATE      0x2C
#define ADXLREG_POWER_CTL    0x2D
#define ADXLREG_INT_ENABLE   0x2E
#define ADXLREG_DATA_FORMAT  0x31
#define ADXLREG_FIFO_CTL     0x38
#define ADXLREG_DUR          0x21

//ADXL345 Data register
#define ADXLREG_DEVID        0x00
#define ADXLREG_DATAX0       0x32
#define ADXLREG_DATAX1       0x33
#define ADXLREG_DATAY0       0x34
#define ADXLREG_DATAY1       0x35
#define ADXLREG_DATAZ0       0x36
#define ADXLREG_DATAZ1       0x37

#define ACC_SENSITIVITY 0.003906f

AccAdxl345::AccAdxl345() {
	// TODO Auto-generated constructor stub

}

AccAdxl345::~AccAdxl345() {
	// TODO Auto-generated destructor stub
}



///**
// * Reference:
// * https://github.com/opendrone/flymaple/blob/testing/src/Accelerometer.cpp
// */
//void Accelerometer::init()
//{
//	uint8_t buff[1];
//	_i2c.readFrom(0x00, 1, buff);
//
//	// now we check msg_data for our 0xE5 magic number
//	uint8_t dev_id = buff[0];
//
//
//	if (dev_id != XL345_DEVID)
//	{
//		logger.error("Error while trying to access Accelerometer\n");
//	}
//
//	//invoke ADXL345
//	_i2c.writeTo(ADXLREG_POWER_CTL,0x00);	HAL::delayMs(5);	//
//	_i2c.writeTo(ADXLREG_POWER_CTL, 0xff);	HAL::delayMs(5);	//
//	_i2c.writeTo(ADXLREG_POWER_CTL, 0x08); HAL::delayMs(5);	//
//	_i2c.writeTo(ADXLREG_DATA_FORMAT, 0x08); HAL::delayMs(5);	//
//	_i2c.writeTo(ADXLREG_BW_RATE, 0x09); HAL::delayMs(5);		//25Hz
//
//
//	HAL::delayMs(100);
//
//	// Calibrate: compute accelerometer scale and offset
//	//	calibration();
//
//	// DEBUG 0 offset
//	_offset = Vect3D(0.0, 0.0, 0.0);
//}
//
//void Accelerometer::update()
//{
//	// Retrieve raw data from I2C
//	uint8_t buff[A_TO_READ];
//
//	_i2c.readFrom(ADXLREG_DATAX0, A_TO_READ, buff);
//
//	int result[3];
//	result[1] = -((((int) buff[1]) << 8) | buff[0]) ;
//	result[0] = -((((int) buff[3]) << 8) | buff[2]) ;
//	result[2] = -((((int) buff[5]) << 8) | buff[4]) ;
//
//	// Create vector 3D from array of int16
//	Vect3D cAcc(result[0], result[1], result[2]);
//
//	// Apply scale factor from LSB to g
//	cAcc *= ACC_SENSITIVITY;
//
//	// Retrieve offset
//	cAcc -= _offset;
//
//	accRaw = cAcc;
//
//	//	_accFiltered = cAcc;
//	accFiltered = accFiltered * (1.0 - _filterNewDataCoeff) + cAcc * _filterNewDataCoeff;
//
//}


