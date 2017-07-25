/*
 * Pwm.cpp
 *
 *  Created on: May 6, 2016
 *      Author: adrien
 */


#include <string.h>
#include "HAL.h"
#include "Pwm.h"


/**
 * Construct pwm pin by finding path of pwm control
 *
 * @param pPinName Pin name
 */
Pwm::Pwm(int pFreqHz, int pPinNumber)
{
	freqHz = pFreqHz;
	pinNumber = pPinNumber;

}

void Pwm::init()
{
	// Already init by bb-setup.sh scripts launch in main function
}

float Pwm::dutyCyclePercent(long dutyCycleMs)
{
	float minDuty = 2.5; // 0° - 800 ms
	float maxDuty = 12.5; // 180° - 2200 ms



}

void Pwm::write(long dutyCycleMs)
{
	// TODO
	//	std::string content = std::to_string((long)(DateUtils::microToNanos(dutyCycleMs)));
	//	FileTools::writeToFile(pwmDutyFilepath, content);

	std::string controlGpio = "./scripts/rasp/gpio.py";
	std::string pinNumberStr = std::to_string(pinNumber);
	std::string dutyCyclePercentStr = std::to_string(dutyCyclePercent(dutyCycleMs));

	std::string gpioCmd = "sudo " + controlGpio + " control " + pinNumberStr + " " + dutyCyclePercentStr;

	/**
	 * TODO currently desactived: Need to implement gpio for rasp in scripts/rasp/
	 */
//	int result = system(gpioCmd.c_str());
}



Pwm::~Pwm()
{

}

