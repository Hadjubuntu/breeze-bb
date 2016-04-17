/*
 * PID.h
 *
 *  Created on: Oct 4, 2015
 *      Author: adrien
 */

#ifndef MATH_PID_PID_H_
#define MATH_PID_PID_H_

class PID {
protected:
	float _Kp;
	float _Ki;
	float _Kd;
	float _maxI;
	float _i;
	float _d;
	float _alphaD;
	float _error;
	float _prevError;
	float _output;

	float _Ke;

	// See papers : http://downloads.deusm.com/designnews/1477-Elsevier06.pdf
	bool _useEnhancePID;
	float _Kboost;
	float _KboostMax;
public:
	PID();
	void init(float pKp, float pKi, float pKd, float pMaxI);
	void setGainParameters(float pKp, float pKi, float pKd);
	void setMaxI(float pMaxI) { _maxI = pMaxI; }
	void update(float error, float dtSeconds);
	float getOutput();
	void reset();
	void printGains();
	float getI();
	float getError();
	float getKe();
	void setUseEnhancePID(bool useEnhancePID);
};

#endif /* MATH_PID_PID_H_ */
