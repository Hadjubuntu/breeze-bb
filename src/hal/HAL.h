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

#include <wirish/wirish.h>


class HAL {
public:
	HAL();
	static void delayMs(unsigned long pDtMs);
	static void delayUs(unsigned long pDtUs);

};

#endif /* PERIPHERALS_HAL_HAL_H_ */
