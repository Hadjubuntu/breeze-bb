/*
 * GeoPosition.cpp
 *
 *  Created on: Sep 18, 2015
 *      Author: adrien
 */

#include "GeoPosition.h"

/**
 * Default constructor
 */
GeoPosition::GeoPosition(float pLat, float pLon, float pAlt) {
	_lat = pLat;
	_lon = pLon;
	_alt = pAlt;
}


float GeoPosition::distance(const GeoPosition& other)
{
return 0.0;
}

float GeoPosition::headingTo(const GeoPosition &other)
{
	float dy = other.getLat() - _lat;
	float dx = other.getLon() - _lon;

	return atan2(dy, dx);
}
