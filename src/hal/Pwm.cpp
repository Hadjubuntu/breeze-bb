/*
 * Pwm.cpp
 *
 *  Created on: May 6, 2016
 *      Author: adrien
 */


#include "HAL.h"
#include "Pwm.h"
#include "../core/FileTools.h"
#include "../core/Constants.h"


/**
 * Construct pwm pin by finding path of pwm control
 *
 * @param pPinName Pin name
 */
Pwm::Pwm(int pFreqHz, int pPinNumber)
{
	freqHz = pFreqHz;
	pinNumber = pPinNumber;

	std::string basePath = "/sys/class/pwm/pwm";
	basePath.append(std::to_string(pinNumber));
	basePath.append("/");

	pwmDutyFilepath = basePath;
	pwmDutyFilepath.append("duty_ns");

	pwmEnableFilepath = basePath;
	pwmEnableFilepath.append("run"); // On debian wheezy / its enable on Jessie..

}

void Pwm::init()
{
	// Already init by bb-setup.sh scripts launch in main function
}

void Pwm::write(long dutyCycleMs)
{
	std::string content = std::to_string(dutyCycleMs * Constants::MS_TO_NS);
	FileTools::writeToFile(pwmDutyFilepath, content);
	printf("Path=%s | Data=%s\n", pwmDutyFilepath, content.c_str());
}

void Pwm::enable()
{
	FileTools::writeToFile(pwmEnableFilepath, std::string("1"));
}

void Pwm::disable()
{
	FileTools::writeToFile(pwmEnableFilepath, std::string("0"));
}



Pwm::~Pwm()
{

}

