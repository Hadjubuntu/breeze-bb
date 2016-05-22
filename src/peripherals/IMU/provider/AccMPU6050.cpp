/*
 * AccMPU6050.cpp
 *
 *  Created on: May 10, 2016
 *      Author: adrien
 */

#include "AccMPU6050.h"

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

AccMPU6050::AccMPU6050() {
	// TODO Auto-generated constructor stub

}

AccMPU6050::~AccMPU6050() {
	// TODO Auto-generated destructor stub
}

void AccMPU6050::init()
{

}

Vect3D AccMPU6050::read()
{
	Vect3D res = Vect3D::zero();
	return res;
}

