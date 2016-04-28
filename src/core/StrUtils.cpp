/**
 * breeze-arm
 * StrUtils.cpp
 * ----------------------------
 *
 * ----------------------------
 *  Created on: Nov 29, 2015
 *      Author: Adrien HADJ-SALAH
 */

#include <string.h>
#include "StrUtils.h"


bool StrUtils::startsWith(const char *s, const char *prefix)
{
	size_t len = strlen(prefix);
	if (strncmp(s, prefix, len) == 0) {
//		return (char *) s + len;
		return true;
	}
	else {
		return false;
	}
}

std::vector<std::string> StrUtils::explode(const std::string& s, const char& c)
{
	std::string buff{""};
	std::vector<std::string> v;

	for(auto n:s)
	{
		if(n != c) buff+=n;
		else if(n == c && buff != "") {
			v.push_back(buff); buff = "";
		}
	}
	if (buff != "") {
		v.push_back(buff);
	}

	return v;
}
