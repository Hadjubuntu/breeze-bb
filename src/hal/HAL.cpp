/**
 * breeze-arm
 * HAL.cpp
 * ----------------------------
 *
 * ----------------------------
 *  Created on: Feb 20, 2016
 *      Author: Adrien HADJ-SALAH
 */

#include <wirish/wirish.h>
#include "HAL.h"

HAL::HAL() {
	// TODO Auto-generated constructor stub

}

void HAL::delayMs(unsigned long pDtMs)
{
	delay(pDtMs);
}

void HAL::delayUs(unsigned long pDtUs)
{
	delayMicroseconds(pDtUs);
}
