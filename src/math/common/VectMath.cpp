/**
 * breeze-arm
 * VectMath.cpp
 * ----------------------------
 *
 * ----------------------------
 *  Created on: Jan 24, 2016
 *      Author: Adrien HADJ-SALAH
 */

#include "VectMath.h"
#include "../filter/LeastSquareFilter.h"

VectMath::VectMath() {
	// TODO Auto-generated constructor stub

}

float VectMath::derivate(std::vector<float> X)
{
	LeastSquareFilter filter;
	float *funcParam = filter.computeLinearFunc(X);

	return funcParam[1];
}

