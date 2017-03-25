/*
 * ActuatorControl.cpp
 *
 *  Created on: Oct 23, 2015
 *      Author: adrien
 */

#include "../../hal/HAL.h"
#include "../../math/common/FastMath.h"
#include "../../data/conf/Conf.h"
#include "ActuatorControl.h"
#include <array>

/**
 * Beaglebone pin mapping for pwm (proto board)
 *
 * proto 0 => pwm 2
 * proto 1 => pwm 0
 * proto 2 => pwm 1
 * proto 3 => pwm 3
 * proto 4 => pwm 4
 * proto 5 => pwm 5
 * proto 6 => pwm 6
 *
 * Timer map
 * Timer 	Ch. 1 pin 	Ch. 2 pin 	Ch. 3 pin 	Ch. 4 pin
Timer1 	6 	7 	8 	–
Timer2 	2 	3 	1 	0
Timer3 	12 	11 	27 	28
Timer4 	5 	9 	14 	24
 */

#define MAX_DELTA_RADIO_SIGNAL 720


#define MOTOR_FREQ_HZ 490 // MOCK FOR SERVO

// This picks the smallest prescaler that allows an overflow < 2^16.
#define CYCLES_PER_MICROSECOND 100000000000000 // FIXME How to manage ESC motors on beaglebone ?
#define MAX_OVERFLOW    ((1 << 16) - 1)
#define CYC_MSEC        (1000 * CYCLES_PER_MICROSECOND)
#define TAU_MSEC        20
#define TAU_USEC        (TAU_MSEC * 1000)
#define TAU_CYC         (TAU_MSEC * CYC_MSEC)
#define SERVO_PRESCALER (TAU_CYC / MAX_OVERFLOW + 1)
#define SERVO_OVERFLOW  ((int)round((double)TAU_CYC / SERVO_PRESCALER))

// Unit conversions
#define US_TO_COMPARE(us) ((int)map((us), 0, TAU_USEC, 0, SERVO_OVERFLOW))
#define COMPARE_TO_US(c)  ((uint32)map((c), 0, SERVO_OVERFLOW, 0, TAU_USEC))

#define HZ_TO_US(f) (1000000.0f/f)

unsigned short levelToCtrl(unsigned short level)
{
	unsigned short ctrl = (level < 0)?0:((level >= 1000)?50000:((1000 + level) * 24));
	return ctrl;
}



/**
 * Processing constructor
 * FIXME configure pwm pin => see bb-setup.sh in scripts directory
 */
ActuatorControl::ActuatorControl(FlightStabilization *pFlightStab, FlightControl *pFlightControl) : Processing(),
		pwm0(Pwm(MOTOR_FREQ_HZ, 2)),
		pwm1(Pwm(MOTOR_FREQ_HZ, 0)),
		pwm2(Pwm(MOTOR_FREQ_HZ, 1)),
		pwm3(Pwm(MOTOR_FREQ_HZ, 3)),
		pwm4(Pwm(MOTOR_FREQ_HZ, 4)),
		smoothPwmRoll(0.0, 0.0),
		smoothPwmPitch(0.0, 0.0)
{
	freqHz = 50;
	_flightStabilization = pFlightStab;
	flightControl = pFlightControl;

	// Retrieve conf params
	_maxCommandNm = Conf::getInstance().get("maxCommandNm");
	_commandNmToSignalUs = Conf::getInstance().get("commandNmToSignalUs");
	motorMinPwmValue = Conf::getInstance().get("motorMinPwmValue");
}

void ActuatorControl::initMotorRepartition() {
	switch (Conf::getInstance().firmware) {
	case YCOPTER:
		for (int j = 0; j < 4; j++) {
			motorActivation[j][2] = 0;
		}

		// Left motor
		motorActivation[0][0] = -1.0;
		motorActivation[0][1] = 1.0;

		// Right motor
		motorActivation[1][0] = 1.0;
		motorActivation[1][1] = 1.0;

		// Rear motor
		motorActivation[2][0] = 0;
		motorActivation[2][1] = -1.8; // Boost coefficient to compensate rear servo weight


		break;
	case FIXED_WING:
		smoothPwmRoll.setParameters(0.0, 0.0, 1000.0);
		smoothPwmPitch.setParameters(0.0, 0.0, 1000.0);
		break;
	case HCOPTER:
		break;
	default:
		break;
	}
}

/**
 * Initialize timers
 */
void ActuatorControl::init()
{
	pwm0.init();
	pwm1.init();
	pwm2.init();
	pwm3.init();
	pwm4.init();

	// Initialize motor repartition especially for Ycopter
	initMotorRepartition();
}


/**
 * Update command to actuators: servo-motors and motors
 */
void ActuatorControl::process()
{
	// Converts 0 to 1 signal to us signal 1000ms wide
	unsigned short int throttle = (_flightStabilization->getThrottle() * 1000);


	switch (Conf::getInstance().firmware)
	{
	case FIXED_WING:
		processFixedWing(throttle);
		break;
	case HCOPTER:
		processMulticopter(throttle, 4);
		break;
	case YCOPTER:
		processMulticopter(throttle, 3);
		break;
	default:
		// Throw error unknow firmware
		break;
	}
}

int ActuatorControl::getCommandNmToSignalUs(float commandNm, float nmToDeltaSignalUs)
{
	BoundAbs(commandNm, _maxCommandNm->getValue());
	int deltaSignal = (int) commandNm * nmToDeltaSignalUs;

	BoundAbs(deltaSignal, MAX_DELTA_RADIO_SIGNAL);
	return deltaSignal;
}

void ActuatorControl::processFixedWing(unsigned short int  throttle)
{
	int motorMinPwm = (int) motorMinPwmValue->getValue();
	int meanPwm = 1450; // TODO put in conf

	RadioControler *radioCtrl = flightControl->getRadioControler();

	if (flightControl->isAutoMode())
	{
		int rollCalib = radioCtrl->getRollCalib();
		int rollMixCalib = radioCtrl->getRollMixCalib();
		int	pitchCalib = radioCtrl->getPitchCalib();
		int	yawCalib = radioCtrl->getYawCalib();

		// Apply flight stabilization output

		// Servos - write pulse
		// -----------------------
		// Wings / Roll
		Vect3D torqueCmd = _flightStabilization->getTau();
		int rollDeltaSignal = getCommandNmToSignalUs(torqueCmd.getX(), 200.0f);
		int pitchDeltaSignal = getCommandNmToSignalUs(torqueCmd.getY(), 200.0f);
		int yawDeltaSignal = getCommandNmToSignalUs(torqueCmd.getZ(), 150.0f);

		smoothPwmRoll.setTarget(rollDeltaSignal);
		smoothPwmPitch.setTarget(pitchDeltaSignal);

		pwm0.write(radioCtrl->getThrottleRawCommand());
		pwm1.write(rollCalib + smoothPwmRoll.getCurrent());
		pwm2.write(rollMixCalib + smoothPwmRoll.getCurrent());
		pwm3.write(pitchCalib + smoothPwmPitch.getCurrent());
		pwm4.write(yawCalib + yawDeltaSignal);
	}
	else {
		// Direct control to the motors and servos

		pwm0.write(radioCtrl->getThrottleRawCommand());
		pwm1.write(radioCtrl->getRollRawCommand());
		pwm2.write(radioCtrl->getRollMixRawCommand());
		pwm3.write(radioCtrl->getPitchRawCommand());
		pwm4.write(radioCtrl->getYawRawCommand());
	}


}


/**
 * H-copter:
 *
 * X1 ------ X2
 *      |
 *      |
 *      |
 * X3 ------ X4
 */
void ActuatorControl::processMulticopter(unsigned short int throttle, int nbMotors)
{
	int motorMinPwm = (int) motorMinPwmValue->getValue();
	// Retrieve torque command
	Vect3D torqueCmd = _flightStabilization->getTau();

	// Compute delta signal from torque command
	int rollDeltaSignal = getCommandNmToSignalUs(torqueCmd.getX(), _commandNmToSignalUs->getValue());
	int pitchDeltaSignal = getCommandNmToSignalUs(torqueCmd.getY(), _commandNmToSignalUs->getValue());
	int yawDeltaSignal = getCommandNmToSignalUs(torqueCmd.getZ(), _commandNmToSignalUs->getValue());

	int motorX[nbMotors];

	for (int j = 0; j < nbMotors;  j ++) {
		motorX[j] = 0;
	}


	// Set throttle repartition only throttle superior to a minimum threshold
	// Otherwise cut-off motors
	if (throttle > 15)
	{
		for (int i = 0; i < nbMotors; i ++)
		{
			motorX[i] = (int)(throttle
					+ motorActivation[i][0] * rollDeltaSignal
					+ motorActivation[i][1] * pitchDeltaSignal
					+ motorActivation[i][2] * yawDeltaSignal);

			Bound(motorX[i], 0, 1000);

			// Debug
			motors[i] = motorX[i];
		}
	}

	pwm0.write(motorMinPwm + motorX[0]);
	pwm1.write(motorMinPwm  + motorX[1]);
	pwm2.write(motorMinPwm  + motorX[2]);

	if (nbMotors == 4)
	{
		pwm3.write(motorMinPwm  + motorX[3]);
	}
	else
	{
		pwm3.write(1450 + yawDeltaSignal);
	}

	// Write pulse for motors
	//	pwmWrite(D28, levelToCtrl(motorX[0]));
	//	pwmWrite(D27, levelToCtrl(motorX[1]));
	//	pwmWrite(D11, levelToCtrl(motorX[2]));
	//
	//	if (nbMotors == 4)
	//	{
	//		pwmWrite(D12, levelToCtrl(motorX[3]));
	//	}
	//	else {
	//		// Signal goes from 650 to 2250 ms
	//		pwmWrite(D14, US_TO_COMPARE(1400 - yawDeltaSignal));
	//	}
}
