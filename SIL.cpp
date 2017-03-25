/*
 * test.cpp
 *
 *  Created on: Feb 19, 2017
 *      Author: adrien-i3
 */

#include <stdio.h>
#include "src/math/frame/Frame.h"
#include "src/core/History.h"
#include "src/math/filter/LeastSquareFilter.h"
#include "src/math/filter/Smoother.h"


History<float> historyValue(10);

int main()
{

	Frame ref = Frame::getEcefFrame();
	Frame uav(&ref, Quaternion::fromEuler(45.0/57.0, 0.0, 0.0), Vect3D(0.0, 0.0, 0.0));

	Vect3D vInertial = uav.fromEcefToInertial(Vect3D(0, 0, 1));
	printf("x=%.2f; y=%.2f; z=%.2f\n", vInertial.getX(), vInertial.getZ(), vInertial.getZ());


	for (int i = 0; i < 15; i ++)
	{
		historyValue.add(i * 1.0f);
	}

	std::vector<float> vectValue = historyValue.toVector();

	for(std::vector<float>::iterator it = vectValue.begin(); it != vectValue.end(); ++it) {
		printf("v:%f\n", (*it));
	}

	LeastSquareFilter filter;
	printf("interpolated: %.2f\n", filter.apply(vectValue, 11));


	Smoother smooth(0.0, 0.0);
	smooth.setParameters(0.0, 0.0, 1.0);
	smooth.setTarget(10.0);
	Date cDate = Date::now();

	while (Date::now().durationFrom(cDate) < 5.0)
	{
		if (Date::now().durationFrom(cDate) > 2.5)
		{
			smooth.setTarget(0.0);
		}
		printf("Smoother : %.1f\n", smooth.getCurrent());
	}



	return 0;
}

