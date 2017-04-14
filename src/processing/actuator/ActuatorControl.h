/*
 * ActuatorControl.h
 *
 *  Created on: Oct 23, 2015
 *      Author: adrien
 */

#ifndef PROCESSING_ACTUATOR_ACTUATORCONTROL_H_
#define PROCESSING_ACTUATOR_ACTUATORCONTROL_H_


#include "../flightstabilization/FlightStabilization.h"
#include "../flightstabilization/FlightControl.h"
#include "../../hal/Pwm.h"
#include "../../math/filter/Smoother.h"

class ActuatorControl : public Processing {
private:
	// conf param
	Param<float> *_maxCommandNm;
	Param<float> *_commandNmToSignalUs;
	Param<float> *motorMinPwmValue;

	// Others param
	FlightStabilization *_flightStabilization;
	FlightControl *flightControl;

	float motorActivation[4][3] = {
			{-1, 1, -1},
			{1, 1, 1},
			{-1, -1, 1},
			{1, -1, -1}
	};

	Pwm pwm0, pwm1, pwm2, pwm3, pwm4;
	Smoother smoothPwmRoll, smoothPwmPitch;

	void initMotorRepartition();

public:
	// debug
	int motors[4];
	ActuatorControl(FlightStabilization *, FlightControl *);

	void init();

	void process();
	void callback() { };

	void processFixedWing(unsigned short int);

	void processMulticopter(unsigned short int, int nbMotors);

	int getCommandNmToSignalUs(float, float);
};

#endif /* PROCESSING_ACTUATOR_ACTUATORCONTROL_H_ */
