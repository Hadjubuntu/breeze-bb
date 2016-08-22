/*
 * RadioControler.h
 *
 *  Created on: Oct 18, 2015
 *      Author: adrien
 */

#ifndef PROCESSING_LINK_RADIOCONTROLER_H_
#define PROCESSING_LINK_RADIOCONTROLER_H_

#include "../../core/Processing.h"
#include "../../hal/Sbus.h"

class RadioControler : public Processing
{
private:
	/** Radio sbus handler */
	Sbus handler;
public:
	/**
	 * Constructor
	 */
	RadioControler();

	/**
	 * Initialize radio
	 */
	void init();

	/**
	 * Update radio output signals
	 */
	void process();
	void callback() { };

	/**
	 * Get radio sbus handler
	 */
	Sbus getHandler()
	{
		return handler;
	}

	/**
	 * Returns last update
	 */
	Date getLastUpdate() {
		return handler.getLastUpdate();
	}
};

#endif /* PROCESSING_LINK_RADIOCONTROLER_H_ */
