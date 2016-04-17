/*
 * Attitude.h
 *
 *  Created on: Oct 5, 2015
 *      Author: adrien
 */

#ifndef MATH_VECTOR_ATTITUDE_H_
#define MATH_VECTOR_ATTITUDE_H_

#include "Vect3D.h"

class Attitude : Vect3D {
private:
	/** Frame in which vector is expressed */
	// Frame _parentFrame;
public:
	Attitude() : Vect3D(Vect3D::zero()) {

	}

	// Vect3D toFrame(Frame f);
};

#endif /* MATH_VECTOR_ATTITUDE_H_ */
