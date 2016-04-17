/*
 * Param.cpp
 *
 *  Created on: Nov 11, 2015
 *      Author: adrien-i3
 */

#include <string>
#include <stdio.h>
#include "Param.h"

template <>
std::string Param<float>::toString()
{
	char buf[80];
	sprintf(buf, "%s;%.5f;", _name.c_str(), _value);
//		std::string str = _name + ";" + std::to_string(_value);

	std::string str(buf);
	return str;
}




template class Param<float>;
template class Param<int>;
template class Param<long>;

