/*
 * GeoPosition.h
 *
 *  Created on: Sep 18, 2015
 *      Author: adrien
 */

#ifndef PROCESSING_NAV_GEOPOSITION_H_
#define PROCESSING_NAV_GEOPOSITION_H_

class GeoPosition {
private:
	/** Latitude in degrees */
	float _lat;
	/** Longitude in radians */
	float _lon;
	/** Altitude in meters */
	float _alt;
public:
	/**
	 * Default constructor
	 */
	GeoPosition(float pLat, float pLon, float pLat);

	/**
	 * Distance to another geo position in meters
	 */
	float distance(const GeoPosition&);

	/**
	 * Heading needed to reach another point (radians)
	 */
	float headingTo(const GeoPosition &);

	/** **************************************
	 *  GETTERS
	 *****************************************/
	float getLat() { return _lat; }
	float getLon() { return _lon; }
};

#endif /* PROCESSING_NAV_GEOPOSITION_H_ */
