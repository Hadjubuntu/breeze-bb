/*
 * Pwm.h
 *
 *  Created on: May 6, 2016
 *      Author: adrien
 */

#ifndef SRC_HAL_PWM_H_
#define SRC_HAL_PWM_H_

#include <string>

/** Pwm Name */
enum PWMName           {
	pWM6                   = 6,
	PWM5                   = 5,
	PWM4                   = 4,
	PWM3                   = 3,
	PWM2                   = 2,
	PWM1                   = 1,
	PWM0                   = 0,

	EHRPWM2B                = PWM5,
	EHRPWM2A                = pWM6,
	EHRPWM1A                = PWM3,
	EHRPWM1B                = PWM4,
	EHRPWM0B                = PWM1,
	EHRPWM0A                = PWM2,
	ECAP0                   = PWM0
};



class Pwm {
private:
	int freqHz;
	int pinNumber;
	std::string pwmDutyFilepath;
	std::string pwmEnableFilepath;
public:
	Pwm(int pFreqHz, int pPin);

	void init();

	void enable();
	void disable();

	void write(long dutyCycleMs);

	virtual ~Pwm();
};

#endif /* SRC_HAL_PWM_H_ */
