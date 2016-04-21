/*
 * RadioControler.h
 *
 *  Created on: Oct 18, 2015
 *      Author: adrien
 */

#ifndef PROCESSING_LINK_RADIOCONTROLER_H_
#define PROCESSING_LINK_RADIOCONTROLER_H_

#include "../../core/Processing.h"
#include "../../link/RadioSbus.h"

class RadioControler : public Processing {
private:
	/** Radio sbus handler */
	RadioSbus _handler;
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
	RadioSbus getHandler()
	{
		return _handler;
	}

	bool isComLost() {
		return _handler.isComLost();
	}

	/**
	 * Returns last update
	 */
	Date& getLastUpdate() {
		return _handler.lastUpdate;
	}
};

#endif /* PROCESSING_LINK_RADIOCONTROLER_H_ */
