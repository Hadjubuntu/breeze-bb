/*
 * Pwm.cpp
 *
 *  Created on: May 6, 2016
 *      Author: adrien
 */

#include "HAL.h"
#include "Pwm.h"

/**
 * Construct pwm pin by finding path of pwm control
 *
 * @param pPinName Pin name
 */
Pwm::Pwm(int pFreqHz, PWMName pPinName)
{
	freqHz = pFreqHz;
	pwmName = pPinName;

	HAL hal;
	capeMgrName = hal.findCapeMgrName();
	opcName = hal.findOpcName();
}

void Pwm::check()
{
	printf("Pwm cape path %s\n", capeMgrName.c_str());
	printf("Opc name %s\n", opcName.c_str());
}



Pwm::~Pwm()
{

}

