/**
 * Breeze project
 * ------------------------------
 * Unmanned vehicule autopilot. Sky is not the limit !
 * ------------------------------
 * (c) Adrien HADJ-SALAH
 */
#include <stdio.h>


#include <string.h>
#include <stdlib.h>

#include "src/data/conf/Conf.h"
#include "src/core/Brain.h"
#include "src/math/time/Date.h"
#include "src/math/common/FastMath.h"
#include "src/processing/ahrs/AHRS.h"
#include "src/processing/link/RadioControler.h"
#include "src/peripherals/IMU/Baro.h"
#include "src/link/RfControler.h"
#include "src/link/RfRouter.h"
#include "src/processing/actuator/ActuatorControl.h"
#include "src/processing/flightstabilization/FlightStabilization.h"
#include "src/processing/flightstabilization/FsAutotune.h"
#include "src/processing/flightstabilization/FlightControl.h"
#include "src/processing/nav/sonar/Sonar.h"
#include "src/processing/link/Telemetry.h"
#include "src/hal/HAL.h"
#include "src/hal/Pwm.h"
#include "src/hal/Uart.h"
#include "src/core/FileTools.h"


bool brainRun = true;


/** Attitude and heading reference system */
Baro baro;
AHRS ahrs(&baro);
Sonar sonar; // Sonar to measure distance

/** OBC */
Brain uavBrain;

/** Data link handler */
RfControler rfControler; // Read/Send packets on UART
RfRouter rfRouter(&rfControler); // Dipatch packet to related controller
RadioControler radioControler; // Sbus radio controller
FlightControl flightControl(&radioControler); // Transform radio signal into radio flight control
Telemetry telemetry(&ahrs, &flightControl, &rfControler); // Build packet to be send to GCS

/** Flight stabilization controller */
FlightStabilization flightStabilization(&ahrs, &flightControl, &sonar);
ActuatorControl actuatorControl(&flightStabilization, &flightControl); // Motor/servo controller

/** Flight stabilization autotune */
//FsAutotune fsAutotune(&flightStabilization);


// Execute script to setup pwm pin and uart
void setupGPIO()
{
	// TODO call new script for Raspberry Pi Zero
	std::string setupGpioFilepath = "./scripts/rasp/gpio.py";
	std::string gpioCmd = "sudo " + setupGpioFilepath + " setup " + Conf::getInstance().firmwareToString();
	printf("Execute script to setup GPIO: %s\n", gpioCmd.c_str());
	int result = system(gpioCmd.c_str());
}

void setup()
{
	// Add dependency (TODO delete this, brain as singleton, call for processing from brain)
	//---------------------
	Conf::getInstance().setRfControler(&rfControler);


	// Setup pwm, uart through bash script
	setupGPIO();

	// Add processings
	//----------------------
	uavBrain.addProcessing(&baro);
	uavBrain.addProcessing(&ahrs); // Calibration done on accelerometer
	uavBrain.addProcessing(&rfControler);
	uavBrain.addProcessing(&rfRouter);
	uavBrain.addProcessing(&radioControler);
	uavBrain.addProcessing(&flightControl);
	uavBrain.addProcessing(&sonar);
	uavBrain.addProcessing(&flightStabilization);
	uavBrain.addProcessing(&actuatorControl);
	//	uavBrain.addProcessing(&telemetry);
	//	uavBrain.addProcessing(&fsAutotune);

	// Initialize all processings
	//----------------------
	printf("Init processings\n");
	uavBrain.initProcessings();
	printf("Processings activated\n");

	std::string logData = "Breeze-bb last execution at " + Date::nowStr() + "\n";
	FileTools::writeToFile("breeze.log", logData);
}

Date start = Date::now();


void loop()
{
	// Call brain loop function to udpate the processings
	// ----
	uavBrain.loop();


	// Prints infos
	// ----
	if (uavBrain.getTickId() % 1000 == 0)
	{
		if (flightControl.isExitCommand())
		{
			printf("Breeze receives exit command\n");
			brainRun = false;
		}

		printf("Breeze iteration: %d\n", uavBrain.getTickId());
		float rpy[3];
		ahrs.getAttitude().toRollPitchYaw(rpy);

		//		char str[90];
		//
		//		sprintf(str, "r=%.1f|p=%.1f|alt=%.1f cm|baro_alt=%.2f", // |baroAlt = %.2f|Temp=%.2f , baro.getAltitudeMeters(), baro.getTemperature()
		//					FastMath::toDegrees(rpy[0]), FastMath::toDegrees(rpy[1]),
		//					baro.getAltitudeMeters()*100.0f);
		//
		//		RfPacket packet(Date::now(), "LOG", str);
		//		rfControler.addPacketToSend(packet);

		float rpyTarget[3];
		flightStabilization.getTargetAttitude().toRollPitchYaw(rpyTarget);
		printf("AHRS [roll=%.2f; pitch=%.2f; yaw=%.2f] | Target[roll=%.2f; pitch=%.2f; yaw=%.2f] | throttle:%.1f\n",
				FastMath::toDegrees(rpy[0]), FastMath::toDegrees(rpy[1]), FastMath::toDegrees(rpy[2]),
				FastMath::toDegrees(rpyTarget[0]), FastMath::toDegrees(rpyTarget[1]), FastMath::toDegrees(rpyTarget[2]),
				flightControl.getThrottleOut());

		printf("stab0:%.1f | stab1:%.1f | stab2:%.1f | X1=%d | gyroRot0=%.1f | gyroRot1=%.1f | auto=%d\n",
				flightStabilization.getTau().getX(),
				flightStabilization.getTau().getY(),
				flightStabilization.getTau().getZ(),
				actuatorControl.motors[0],
				ahrs.getGyroHyperFiltered().getX() * 57.0,
				ahrs.getGyroHyperFiltered().getY() * 57.0,
				flightControl.isAutoMode()
		);
	}
}

int main(int argc, char *argv[])
{
	printf("Start setup");
	setup();
	printf("................done\n");

	printf("Start brain\n");
	while (brainRun) {
		loop();
	}
	printf("Brain......................shutdown\n");
	return 0;
}
