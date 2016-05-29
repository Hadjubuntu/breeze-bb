/*
 * Pwm.cpp
 *
 *  Created on: May 6, 2016
 *      Author: adrien
 */
#include <sstream>
#include "HAL.h"
#include "Pwm.h"
#include "../core/FileTools.h"
#include "../core/Constants.h"

/**
 * Construct pwm pin by finding path of pwm control
 *
 * @param pPinName Pin name
 */
Pwm::Pwm(int pFreqHz, PWMName pPinName)
{
	freqHz = pFreqHz;
	pwmName = pPinName;
	scriptDirectory = findScriptDirectory();
}

void Pwm::init()
{
	setupPwmAndDisable(pwmName, freqHz);
}

void Pwm::setupPwmAndDisable(PWMName pinNumber, int freqHz)
{
	long periodNs = (long) (Constants::S_TO_NS / freqHz);
	std::stringstream periodStr;
	periodStr << periodNs;

	std::string defaultDutyCycle = "900000"; // 900 ms

	char parameters[100];
	sprintf(parameters, "%d %d", mapPwmPin[pinNumber], mapPwmSubpin[pinNumber]);

	std::string paramStr = "";
	paramStr.append(parameters);
	paramStr.append(" " + periodStr.str() + " " + defaultDutyCycle);

	std::string scriptSetup = "sudo ./" + scriptDirectory + "/pwm-setup.sh " + paramStr;

	// Execute script to setup pwm pin
	int result = system(scriptSetup.c_str());
	printf("Executed command setup pwm %s\n", scriptSetup.c_str());
}

void Pwm::write(long dutyCycleMs)
{

}

std::string Pwm::findScriptDirectory()
{
	std::string searchResult = FileTools::searchDirectory(".","scripts");
	return searchResult;
}

void Pwm::enable()
{

}

void Pwm::disable()
{

}

void Pwm::check()
{
//	printf("Script dir : %s\n", scriptDirectory.c_str());
}



Pwm::~Pwm()
{

}

