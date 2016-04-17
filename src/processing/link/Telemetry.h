/*
 * Telemetry.h
 *
 *  Created on: Nov 8, 2015
 *      Author: adrien-i3
 */

#ifndef PROCESSING_LINK_TELEMETRY_H_
#define PROCESSING_LINK_TELEMETRY_H_

#include "../../core/Processing.h"
#include "../ahrs/AHRS.h"
#include "../flightstabilization/FlightControl.h"
#include "../../link/RfControler.h"

class Telemetry : public Processing {
private:
	AHRS *_ahrs;
	FlightControl *_flightControl;
	RfControler *_rfControler;
public:
	Telemetry(AHRS*, FlightControl*, RfControler*);

	void init();
	void process();
	void callback() { };
};

#endif /* PROCESSING_LINK_TELEMETRY_H_ */
