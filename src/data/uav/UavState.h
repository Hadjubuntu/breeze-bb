/*
 * UavState.h
 *
 *  Created on: Sep 15, 2015
 *      Author: adrien
 */

#ifndef PROCESSING_UAV_UAVSTATE_H_
#define PROCESSING_UAV_UAVSTATE_H_

#include "../../math/vector/Quaternion.h"

class UavState {
private:
	// Date related to the state
	Date _date;

	// GeoPosition
	GeoPosition _geoPosition;

	// AHRS state
	Quaternion _attitude;

	// Speed
	float _speedMs;

public:
	UavState();
};

#endif /* PROCESSING_UAV_UAVSTATE_H_ */
