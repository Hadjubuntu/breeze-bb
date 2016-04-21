/*
 * Quaternion.h
 *
 *  Created on: Sep 22, 2015
 *      Author: adrien
 */

#ifndef MATH_VECTOR_QUATERNION_H_
#define MATH_VECTOR_QUATERNION_H_

#include "Vect3D.h"

/**
 * Quaternion
 */
class Quaternion {
private:
	float _w;
	float _x;
	float _y;
	float _z;
public:
	/**
	 * Constructor of a quaternion
	 */
	Quaternion(float w, float x, float y, float z) : _w(w), _x(x), _y(y), _z(z) {
		// Default constructor
	}

	Quaternion(float roll, float pitch, float yaw);

	/**
	 * Quaternion to roll pitch yaw
	 */
	void toRollPitchYaw(float *rpy);
	Vect3D toRollPitchYawVect3D();

	/** Conjugate quaternion */
	Quaternion conjugate() {
		Quaternion e(_w, -_x, -_y, -_z);
		return e;
	}

	Quaternion inv() {
		Quaternion conj = (*this).conjugate();
		return (conj / conj.getNorm2());
	}

	Vect3D getVect3DPart() {
		Vect3D e(_x, _y, _z);
		return e;
	}

	Vect3D rotate(Vect3D pVector);

	/* ********************************
	 * Norm
	 ******************************** */
	float getNorm2();
	float getNorm();

	/* ********************************
	 * Operators
	 ******************************** */

	Quaternion operator/(float v)
	{
		Quaternion e(_w/v, _x/v, _y/v,  _z/v);
		return e;
	}

	Quaternion operator*(Quaternion q) {
		float w = _w * q._w - _x * q._x - _y * q._y - _z * q._z;
		float x = _w * q._x + _x * q._w + _y * q._z - _z * q._y;
		float y = _w * q._y - _x * q._z + _y * q._w + _z * q._x;
		float z = _w * q._z + _x * q._y - _y * q._x + _z * q._w;

		Quaternion e(w, x, y, z);
		return e;
	}

	Quaternion operator*=(Quaternion q) {
		(*this) = (*this) * q;
		return (*this);
	}

	float operator[](int x) {
		if (x == 0) {
			return _w;
		} else if (x == 1) {
			return _x;
		} else if (x == 2) {
			return _y;
		} else if (x == 3) {
			return _z;
		} else {
			// TODO throw error
			return 0.0;
		}
	}

	Quaternion operator+=(float array[]) {
		_w += array[0];
		_x += array[1];
		_y += array[2];
		_z += array[3];
		return (*this);
	}
	Quaternion operator-=(float array[]) {
		_w -= array[0];
		_x -= array[1];
		_y -= array[2];
		_z -= array[3];
		return (*this);
	}
	Quaternion operator*=(float v) {
		_w *= v;
		_x *= v;
		_y *= v;
		_z *= v;
		return (*this);
	}
	Quaternion operator=(Quaternion q) {
		_w = q[0];
		_x = q[1];
		_y = q[2];
		_z = q[3];
		return (*this);
	}
	Quaternion operator-() {
		_w = -_w;
		_x = -_x;
		_y = -_y;
		_z = -_z;
		return (*this);
	}

	/**
	 * Construct quaternion from euler angles
	 */
	static Quaternion fromEuler(float rollRad, float pitchRad, float yawRad) {
		return Quaternion(rollRad, pitchRad, yawRad);
	}


	static Quaternion zero() {
		Quaternion e(1.0, 0.0, 0.0, 0.0);
		return e;
	}
};

#endif /* MATH_VECTOR_QUATERNION_H_ */
