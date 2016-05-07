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
	P8_13                   = 0,
	P8_19                   = 1,
	P9_14                   = 2,
	P9_16                   = 3,
	P9_21                   = 4,
	P9_22                   = 5,
	P9_42                   = 6,

	EHRPWM2B                = P8_13,
	EHRPWM2A                = P8_19,
	EHRPWM1A                = P9_14,
	EHRPWM1B                = P9_16,
	EHRPWM0B                = P9_21,
	EHRPWM0A                = P9_22,
	ECAP0                   = P9_42
};

class Pwm {
private:
	int freqHz;
	PWMName pwmName;
	std::string capeMgrName;
	std::string opcName;
public:
	Pwm(int pFreqHz, PWMName pName);

	void check();

	virtual ~Pwm();
};

#endif /* SRC_HAL_PWM_H_ */
