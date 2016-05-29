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
	PWM5                   = 5,
	pWM6                   = 6,
	PWM3                   = 3,
	PWM4                   = 4,
	PWM1                   = 1,
	PWM2                   = 2,
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
