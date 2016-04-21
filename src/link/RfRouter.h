/*
 * RfRouter.h
 *
 *  Created on: Oct 16, 2015
 *      Author: adrien
 */

#ifndef LINK_RFROUTER_H_
#define LINK_RFROUTER_H_

#include "RfControler.h"

class RfRouter: public Processing {
private:
	RfControler *_controler;
public:
	RfRouter(RfControler *);

	void init() {
		// TODO
	}
	void process();
	void callback() {

	};
};

#endif /* LINK_RFROUTER_H_ */
