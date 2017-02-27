/*
 * Vect3.h
 *
 *  Created on: Sep 23, 2015
 *      Author: adrien
 */

#ifndef MATH_VECTOR_VECT3D_H_
#define MATH_VECTOR_VECT3D_H_

#include "../../hal/HalTypes.h"

class Vect3D {
private:
	float _x;
	float _y;
	float _z;
public:
	/** Default constructor */
	Vect3D(float x, float y, float z) : _x(x), _y(y), _z(z) {

	}

	/** *************************************************
	 * Basic functions on vectors
	 ************************************************** */
	float dotProduct(Vect3D& vect) {
		return _x * vect.getX() + _y * vect.getY() + _z * vect.getZ();
	}

	Vect3D crossProduct(Vect3D& vect) {
		Vect3D e(_y * vect._z - _z * vect._y,
				_z * vect._x - _x * vect._z,
				_x * vect._y - _y * vect._x);
		return e;
	}

	Vect3D toRad();
	Vect3D toDeg();

	Vect3D roundvect();

	/** *************************************************
	 * Operators
	 ************************************************** */
	Vect3D operator*(float scalar) const {
		return Vect3D(_x * scalar, _y * scalar, _z * scalar);
	}
	Vect3D operator/(float scalar) const {
		if (scalar > 1e-6 || scalar < -1e-6) {
		return Vect3D(_x / scalar, _y / scalar, _z / scalar);
		}
		else {
			return Vect3D::zero();
		}
	}
	Vect3D operator+(const Vect3D &vect) const {
		return Vect3D(_x + vect._x, _y + vect._y, _z + vect._z);
	}
	Vect3D operator-(const Vect3D vect) const {
		return Vect3D(_x - vect._x, _y - vect._y, _z - vect._z);
	}
	Vect3D operator*(const Vect3D &vect) const {
		return Vect3D(_x * vect._x, _y * vect._y, _z * vect._z);
	}
	Vect3D operator/(const Vect3D &vect) const {
		return Vect3D(_x / vect._x, _y / vect._y, _z / vect._z);
	}
	float operator[](int i) const {
		if (i == 0) {
			return _x;
		} else if (i == 1) {
			return _y;
		}
		else {
			return _z;
		}
	}
	Vect3D operator=(Vect3D vect) {
		_x = vect._x;
		_y = vect._y;
		_z = vect._z;
		return (*this);
	}
	Vect3D operator=(float array[]) {
		_x = array[0];
		_y = array[1];
		_z = array[2];
		return (*this);
	}
//	Vect3D operator=(int16 array[]) {
//		_x = (float) array[0];
//		_y = (float) array[1];
//		_z = (float) array[2];
//		return (*this);
//	}
	Vect3D operator+=(Vect3D vect)	{
		_x += vect._x;
		_y += vect._y;
		_z += vect._z;
		return (*this);
	}
	Vect3D operator-=(Vect3D vect)	{
		_x -= vect._x;
		_y -= vect._y;
		_z -= vect._z;
		return (*this);
	}
	Vect3D operator*=(float scalar) {
		_x *= scalar;
		_y *= scalar;
		_z *= scalar;
		return (*this);
	}
	/** *************************************************
	 * Setter
	 ************************************************** */
//	void setX(float x) { _x = x; }
//	void setY(float y) { _y = y; }
//	void setZ(float z) { _z = z; }

	/** *************************************************
	 * Getters
	 ************************************************** */
	float getX() {
		return _x;
	}

	float getY() {
		return _y;
	}

	float getZ() {
		return _z;
	}

	float getNorm2() {
		return _x*_x + _y*_y + _z*_z;
	}

	/** *************************************************
	 * Instance creator
	 ************************************************** */
	static Vect3D zero() {
		Vect3D zero(0.0, 0.0, 0.0);
		return zero;
	}
//	static Vect3D fromInt16Array(int16 array[])
//	{
//		Vect3D e((float) array[0], (float) array[1], (float) array[2]);
//		return e;
//	}
};

#endif /* MATH_VECTOR_VECT3D_H_ */
