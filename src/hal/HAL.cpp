/**
 * breeze-arm
 * HAL.cpp
 * ----------------------------
 *
 * ----------------------------
 *  Created on: Feb 20, 2016
 *      Author: Adrien HADJ-SALAH
 */

#include "../math/time/Date.h"
#include "../core/Constants.h"
#include "HAL.h"

HAL::HAL() {
	// TODO Auto-generated constructor stub

}

void HAL::delayMs(unsigned long pDtMs)
{
	//	FIXME for beaglebone delay(pDtMs);
}

void HAL::delayUs(unsigned long pDtUs)
{

	Date d = Date::now();
	while (Date::now().durationFrom(d)*Constants::S_TO_US < pDtUs) {
		// Wait
	}
}
