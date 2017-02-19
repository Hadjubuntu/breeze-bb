/*
 * test.cpp
 *
 *  Created on: Feb 19, 2017
 *      Author: adrien-i3
 */

#include <stdio.h>
#include "src/math/frame/Frame.h"

int main()
{

	Frame ref = Frame::getEcefFrame();
	Frame uav(&ref, Quaternion::fromEuler(45.0/57.0, 0.0, 0.0), Vect3D(0.0, 0.0, 0.0));

	Vect3D vInertial = uav.fromEcefToInertial(Vect3D(0, 0, 1));
	printf("x=%.2f; y=%.2f; z=%.2f\n", vInertial.getX(), vInertial.getZ(), vInertial.getZ());


	printf("Test 2\n");
	return 0;
}

