/*
 * Vect2D.h
 *
 *  Created on: Sep 21, 2015
 *      Author: adrien
 */

#ifndef MATH_VECTOR_VECT2D_H_
#define MATH_VECTOR_VECT2D_H_

class Vect2D {
private:
	float _x;
	float _y;
public:
	/**
	 * Default constructor
	 */
	Vect2D(float x, float y) : _x(x), _y(y) {

	}

	/** *************************************************
	 * Basic functions on vectors
	 ************************************************** */
	float dotProduct(Vect2D& vect) {
		return _x * vect.getX() + _y * vect.getY();
	}

	float crossProduct(Vect2D& vect) {
		return _x * vect.getY() - _y * vect.getX();
	}

	/** *************************************************
	 * Operators
	 ************************************************** */
	Vect2D operator*(float scalar) const {
		return Vect2D(_x * scalar, _y * scalar);
	}
	Vect2D operator+(const Vect2D &vect) const {
		return Vect2D(_x + vect._x, _y + vect._y);
	}
	Vect2D operator-(const Vect2D &vect) const {
		return Vect2D(_x - vect._x, _y - vect._y);
	}
	Vect2D operator*(const Vect2D &vect) const {
		return Vect2D(_x * vect._x, _y * vect._y);
	}
	Vect2D operator/(const Vect2D &vect) const {
		return Vect2D(_x / vect._x, _y / vect._y);
	}

	/** *************************************************
	 * Getters
	 ************************************************** */
	float getX() {
		return _x;
	}

	float getY() {
		return _y;
	}
};

#endif /* MATH_VECTOR_VECT2D_H_ */
