/*
 * Constants.h
 *
 *  Created on: Apr 28, 2016
 *      Author: adrien-i3
 */

#ifndef SRC_CORE_CONSTANTS_H_
#define SRC_CORE_CONSTANTS_H_

class Constants {
public:
	static constexpr float S_TO_US = 1000000.0;
	static constexpr float S_TO_NS = 1000000000.0;
	static constexpr float MS_TO_NS = 1000000.0;
	Constants();
	virtual ~Constants();
};

#endif /* SRC_CORE_CONSTANTS_H_ */
