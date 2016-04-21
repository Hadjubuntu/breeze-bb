/**
 * breeze-arm
 * LeastSquare.cpp
 * ----------------------------
 *
 * ----------------------------
 *  Created on: Jan 22, 2016
 *      Author: Adrien HADJ-SALAH
 */

#include "LeastSquareFilter.h"

LeastSquareFilter::LeastSquareFilter()
{
	// Default constructor
}

float* LeastSquareFilter::computeLinearFunc(std::vector<float> Y)
{
	int n = Y.size();
	float res[2];
	res[0] = 0.0;
	res[1] = 0.0;

	if (n > 0) {
		std::vector<float> X = createSimpleVector(n);

		float meanX = mean(X);
		std::vector<float> x_minus_meanx = addScalar(X, -meanX);
		std::vector<float> x_minus_meanx_square = product(x_minus_meanx, x_minus_meanx);
		float SSxx = sum(x_minus_meanx_square);

		//
		float sumY = sum(Y);
		float sumX = n * (n-1) / 2.0;
		std::vector<float> x_product_y = product(X, Y);
		float SSxy = sum(x_product_y) - sumX * sumY / n;
		//
		float b1 = 0.0;

		if (SSxx != 0.0) {
			b1 = SSxy / SSxx;
		}
		float b0 = sumY / n - b1 * sumX / n;

		res[0] = b0;
		res[1] = b1;
	}

	return res;
}

float LeastSquareFilter::apply(std::vector<float> Y, int idx)
{
	// Compute linear function parameter
	float *func = computeLinearFunc(Y);

	// Use idx + 1 to simulate "future"
	return func[0] + func[1] * (idx+1);
}

std::vector<float> LeastSquareFilter::createSimpleVector(int n)
{
	std::vector<float> res;
	res.reserve(n);
	for (int i = 0; i < n; i ++)
	{
		res.push_back((float) i);
	}

	return res;
}

float LeastSquareFilter::mean(std::vector<float> pX)
{
	int n = pX.size();
	int sum = 0;
	for (int v: pX) {
		sum += v;
	}

	return ((float)sum) / n;
}

std::vector<float> LeastSquareFilter::addScalar(std::vector<float> pVect, float pScalar)
{
	std::vector<float> res;
	int n = pVect.size();
	res.reserve(n);
	for (int v: pVect)
	{
		res.push_back(v + pScalar);
	}

	return res;
}

std::vector<float>  LeastSquareFilter::product(std::vector<float> A, std::vector<float> B)
{
	std::vector<float> res;
	int n = A.size();
	res.reserve(n);
	for (int i=0; i < n; i ++)
	{
		res.push_back(A.at(i) * B.at(i));
	}

	return res;
}

float LeastSquareFilter::sum(std::vector<float> A)
{
	float res = 0.0;
	for (float v : A)
	{
		res += v;
	}

	return res;
}
