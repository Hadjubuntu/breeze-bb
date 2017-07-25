/*
 * Pwm.h
 *
 *  Created on: May 6, 2016
 *      Author: adrien
 */

#ifndef SRC_HAL_PWM_H_
#define SRC_HAL_PWM_H_




class Pwm {
private:
	int freqHz;
	int pinNumber;
public:
	Pwm(int pFreqHz, int pPin);

	void init();

	void enable();
	void disable();

	void write(long dutyCycleMs);

	float dutyCyclePercent(long dutyCycleMs);

	virtual ~Pwm();
};

#endif /* SRC_HAL_PWM_H_ */
