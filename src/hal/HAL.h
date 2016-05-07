/**
 * breeze-arm
 * HAL.h
 * ----------------------------
 *
 * ----------------------------
 *  Created on: Feb 20, 2016
 *      Author: Adrien HADJ-SALAH
 */

#ifndef PERIPHERALS_HAL_HAL_H_
#define PERIPHERALS_HAL_HAL_H_


#include <string>

#define HAL_GPIO_INPUT 0
#define HAL_GPIO_OUTPUT 1


class HAL {
public:
	HAL();

	std::string findCapeMgrName();
	std::string findOpcName();

	static void delayMs(unsigned long pDtMs);
	static void delayUs(unsigned long pDtUs);

};

#endif /* PERIPHERALS_HAL_HAL_H_ */
