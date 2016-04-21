/*
 * Vect3.cpp
 *
 *  Created on: Sep 23, 2015
 *      Author: adrien
 */

#include <cmath>
#include "Vect3D.h"
#include "../common/FastMath.h"

Vect3D Vect3D::toRad()
{
	Vect3D e(
			FastMath::toRadians(_x),
			FastMath::toRadians(_y),
			FastMath::toRadians(_z));
	return e;
}


Vect3D Vect3D::roundvect()
{
	_x = round(_x);
	_y = round(_y);
	_z = round(_z);
	return (*this);
}
