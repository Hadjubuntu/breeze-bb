/*
 * Quaternion.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: adrien
 */

#include <math.h>
#include "../common/FastMath.h"
#include "../../core/Logger.h"
#include "Quaternion.h"


/**
 * Constructor from roll pitch yaw
 * @param rollRad roll [radians]
 * @param pitchRad pitch [radians]
 * @param yawRad yaw [radians]
 * */
Quaternion::Quaternion(float rollRad, float pitchRad, float yawRad)
{
	float phi, theta, psi;
	float cphi, sphi, ctheta, stheta, cpsi, spsi;

	phi    = rollRad / 2.0;
	theta  = pitchRad / 2.0;
	psi    = yawRad / 2.0;
	cphi   = cosf(phi);
	sphi   = sinf(phi);
	ctheta = cosf(theta);
	stheta = sinf(theta);
	cpsi   = cosf(psi);
	spsi   = sinf(psi);

	_w   = cphi * ctheta * cpsi + sphi * stheta * spsi;
	_x   = sphi * ctheta * cpsi - cphi * stheta * spsi;
	_y   = cphi * stheta * cpsi + sphi * ctheta * spsi;
	_z   = cphi * ctheta * spsi - sphi * stheta * cpsi;

	if (_w < 0) { // q0 always positive for uniqueness
		_w = -_w;
		_x = -_x;
		_y = -_y;
		_z = -_z;
	}
}

float Quaternion::operator[](int x) {
		if (x == 0) {
			return _w;
		} else if (x == 1) {
			return _x;
		} else if (x == 2) {
			return _y;
		} else if (x == 3) {
			return _z;
		} else {
			Logger::getDefault().error("Error while using [] operator on quaternion - out of bounds");
			return 0.0;
		}
	}

void Quaternion::toRollPitchYaw(float rpy[3])
{
	float R13, R11, R12, R23, R33;
	float q0s = _w * _w;
	float q1s = _x * _x;
	float q2s = _y * _y;
	float q3s = _z * _z;

	R13    = 2.0 * (_x * _z - _w * _y);
	R11    = q0s + q1s - q2s - q3s;
	R12    = 2.0 * (_x * _y + _w * _z);
	R23    = 2.0 * (_y * _z + _w * _x);
	R33    = q0s - q1s - q2s + q3s;

	rpy[1] = asinf(-R13); // pitch always between -pi/2 to pi/2
	rpy[2] = atan2f(R12, R11);
	rpy[0] = atan2f(R23, R33);
}

Vect3D Quaternion::toRollPitchYawVect3D() {
	float rpy[3];
	toRollPitchYaw(rpy);

	return Vect3D(rpy[0], rpy[1], rpy[2]);
}

float Quaternion::getNorm2() {
	return _w *_w + _x*_x +_y*_y + _z*_z;
}

float Quaternion::getNorm() {
	return FastMath::fsqrt(getNorm2());
}

// See: http://fr.mathworks.com/help/aeroblks/quaternionrotation.html?requestedDomain=www.mathworks.com
Vect3D Quaternion::rotate(Vect3D v)
{

    float _y_y = _y * _y;
    float _y_z = _y * _z;
    float _x_x = _x * _x;
    float _x_y = _x * _y;
    float _x_z = _x * _z;
    float _w_x = _w * _x;
    float _w_y = _w * _y;
    float _w_z = _w * _z;
    float _z_z = _z * _z;

    float max, may, maz, mbx, mby, mbz, mcx, mcy, mcz;
    max = 1.0f-2.0f*(_y_y + _z_z);
    may =   2.0f*(_x_y - _w_z);
    maz =   2.0f*(_x_z + _w_y);

    mbx =   2.0f*(_x_y + _w_z);
    mby = 1.0f-2.0f*(_x_x + _z_z);
    mbz =   2.0f*(_y_z - _w_x);

    mcx =   2.0f*(_x_z - _w_y);
    mcy =   2.0f*(_y_z + _w_x);
    mcz = 1.0f-2.0f*(_x_x + _y_y);

    float resX, resY, resZ;
    resX = max * v.getX() + mbx * v.getY() + mcx * v.getZ();
    resY = may * v.getX() + mby * v.getY() + mcy * v.getZ();
    resZ = maz * v.getX() + mbz * v.getY() + mcz * v.getZ();

    return Vect3D(resX, resY, resZ);
}

