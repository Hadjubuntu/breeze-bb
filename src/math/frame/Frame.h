/*
 * Frame.h
 *
 *  Created on: Feb 13, 2017
 *      Author: adrien-i3
 */

#ifndef SRC_MATH_FRAME_FRAME_H_
#define SRC_MATH_FRAME_FRAME_H_

#include "../vector/Quaternion.h"
#include "../vector/Vect3D.h"



class Frame {
protected:
	Frame *parent;
	Quaternion rotation;
	Vect3D translation;
public:
	Frame() : parent(0), rotation(Quaternion::zero()), translation(Vect3D::zero())
	{
		// Initialize frame
	}

	Frame(Frame *, Quaternion, Vect3D);

	/**
	 * Transforms a vector input into an output vector depending on the source frame
	 * TODO to be more generic
	 */
	Vect3D fromEcefToInertial(Vect3D pInput)
	{
		Vect3D output = rotation.conjugate().rotate(pInput);
		output += translation;

		return output;
	}
	Vect3D fromInertialToEcef(Vect3D pInput)
	{
		Vect3D output = rotation.rotate(pInput);
		output -= translation;

		return output;
	}

	/**
	 * Getters
	 */
	Frame* getParent()
	{
		return parent;
	}

	/**
	 * Setters
	 */
	void setParent(Frame *pParent)
	{
		parent = pParent;
	}
	void setRotation(Quaternion pQuat)
	{
		rotation = pQuat;
	}
	void setTranslation(Vect3D pVect3D)
	{
		translation = pVect3D;
	}

	bool isEcef() {
		return !parent;
	}

	/**
	 * Static functions
	 */
	static Frame getEcefFrame()
	{
		Frame e;
		e.setRotation(Quaternion::zero());
		e.setTranslation(Vect3D::zero());

		return e;
	}
};

#endif /* SRC_MATH_FRAME_FRAME_H_ */
