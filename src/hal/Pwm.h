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
	P8_13                   = 5,
	P8_19                   = 6,
	P9_14                   = 3,
	P9_16                   = 4,
	P9_21                   = 1,
	P9_22                   = 2,
	P9_42                   = 0,

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
	int mapPwmPin[7]={0, 2, 2, 4, 4, 6, 6};
	int mapPwmSubpin[7]={0, 0, 1, 0, 1, 0, 1};
	int freqHz;
	PWMName pwmName;
	std::string scriptDirectory;

	void setupPwmAndDisable(PWMName, int);
public:
	Pwm(int pFreqHz, PWMName pName);

	void init();
	void check();
	std::string findScriptDirectory();

	void enable();
	void disable();

	void write(long dutyCycleMs);

	virtual ~Pwm();
};

#endif /* SRC_HAL_PWM_H_ */
