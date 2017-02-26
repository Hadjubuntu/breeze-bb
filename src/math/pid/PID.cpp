/*
 * PID.cpp
 *
 *  Created on: Oct 4, 2015
 *      Author: adrien
 */

#include "../common/FastMath.h"
#include "PID.h"


PID::PID() // : errorIntegral(RingFloatArray(100))
{
	init(1.0, 0.1, 0.01, 10);
}

void PID::init(float pKp, float pKd, float pKi, float pMaxI) {
	_Kp = pKp;
	_Ki = pKi;
	_Kd = pKd;
	_maxI = pMaxI;
	_i = 0.0;
	_d = 0.0;
	_alphaD = 0.6;
	_prevError = 0.0;
	_output = 0.0;
	_useEnhancePID = false;
	_Kboost = 0.012;
	_KboostMax = 2.0;
	_Ke = 1.0;
}

void PID::setGainParameters(float pKp, float pKd, float pKi)
{
	_Kp = pKp;
	_Kd = pKd;
	_Ki = pKi;
}


void PID::reset()
{
	_i = 0.0;
	_d = 0.0;
	_prevError = 0.0;
}

void PID::update(float e, float dtSeconds)
{
	_error = e;

	// Evaluate differential
	float dError = 0.0;
	if (dtSeconds > 0.0) {
		dError = (_error - _prevError) / dtSeconds;
	}
	_prevError = _error;

	// Evaluate integral
	_i = _i + _Ki * _error * dtSeconds;
	BoundAbs(_i, _maxI);

	_d = (1.0 - _alphaD) * _d + _alphaD * dError;


	if (_useEnhancePID)
	{
		// High when error is high :
//		_Ke = (FastMath::exp(_Kboost * _error) + FastMath::exp(-_Kboost * _error)) / 2.0;
//		Bound(_Ke, 1.0, _KboostMax);
		_Ke = 1.0;
	}
	else {
		_Ke = 1.0;
	}

	// Computes output
	_output = _Ke * (_Kp * _error +  _i + _Kd * _d);

	// Store data in integral
//	errorIntegral.add(_error);
}

void PID::setUseEnhancePID(bool pUseEnhancePID)
{
	_useEnhancePID = pUseEnhancePID;
}

float PID::getOutput()
{
	return _output;
}

float PID::getI() {
	return _i;
}

float PID::getError() {
	return _error;
}

float PID::getKe() {
	return _Ke;
}
