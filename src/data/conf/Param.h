/*
 * Param.h
 *
 *  Created on: Nov 11, 2015
 *      Author: adrien-i3
 */

#ifndef DATA_CONF_PARAM_H_
#define DATA_CONF_PARAM_H_

#include <string>

template <class T>
class Param {
private:
	std::string _name;
	T _value;
public:
	Param(std::string pName, T pValue) : _name(pName), _value(pValue)
{

}

	std::string getName() {
		return _name;
	}

	T getValue() {
		return _value;
	}

	void setValue(T value) {
		_value = value;
	}

	std::string toString();

	virtual ~Param() {

	}
};

#endif /* DATA_CONF_PARAM_H_ */
