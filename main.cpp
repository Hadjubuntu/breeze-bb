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


/** Attitude and heading reference system */
Baro baro;
AHRS ahrs(&baro);

/** UAV brain */
Brain uavBrain;

/** RadioFreq controller */
RfControler rfControler;

/** RadioFreq router */
RfRouter rfRouter(&rfControler);

/** Radio controller */
RadioControler radioControler;

/** Transform radio signal into radio flight control */
FlightControl flightControl(&radioControler);

/** Sonar to measure distance */
Sonar sonar;

/** Flight stabilization controller */
FlightStabilization flightStabilization(&ahrs, &flightControl, &sonar);

/** Motor and servo control */
ActuatorControl actuatorControl(&flightStabilization);

/** Telemetry to keep GCS update */
//Telemetry telemetry(&ahrs, &flightControl, &rfControler);

/** Flight stabilization autotune */
//FsAutotune fsAutotune(&flightStabilization);


void calibration()
{
	int nbCalibrationMeasure = 100;

	for (int i = 0; i < nbCalibrationMeasure; i++)
	{
		uavBrain.loop();
		//		ahrs.calibrateOffset();

		HAL::delayMs(10);
	}
}
// Execute script to setup pwm pin and uart
void setupPwmUart()
{
	std::string scriptDirectory = FileTools::searchDirectory(".","scripts");
	std::string scriptSetup = "sudo ./" + scriptDirectory + "/bb-setup.sh ";
	int result = system(scriptSetup.c_str());
	printf("Setup pwm and uart results=%d\n", result);
}

void setup()
{
	// Add dependency (TODO delete this, brain as singleton, call for processing from brain)
	//---------------------
	Conf::getInstance().setRfControler(&rfControler);

	// Initialize brain
	//---------------------
	uavBrain.enableI2C();

	// Add processings
	//----------------------
	uavBrain.addProcessing(&baro);
	uavBrain.addProcessing(&ahrs);
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
	uavBrain.initProcessings();

	// Calibration on AHRS
	//----------------------
	calibration();

	// Setup pwm, uart through bash script
	setupPwmUart();
}



void loop()
{
	// Call brain loop function to udpate the processings
	// ----
	uavBrain.loop();

	// Prints infos
	// ----
	if (uavBrain.getTickId() % 3000 == 0)
	{
		printf("brain loop\n");
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

		//
		printf("AHRS [roll=%.2f; pitch=%.2f] | baro=%.1f\n",
				FastMath::toDegrees(rpy[0]), FastMath::toDegrees(rpy[1]), baro.getAltitudeMeters()*100.0f);
	}
}

int main(int argc, char *argv[])
{
	int iter = 0;

	printf("Start setup");
	setup();
	printf("................done\n");

	printf("Start brain\n");
	while (iter < 1000000) {
		loop();
		iter ++;
	}
	printf("Brain......................shutdown\n");
	return 0;
}
