/**
 * breeze-arm
 * StrUtils.h
 * ----------------------------
 *
 * ----------------------------
 *  Created on: Nov 29, 2015
 *      Author: Adrien HADJ-SALAH
 */

#ifndef CORE_STRUTILS_H_
#define CORE_STRUTILS_H_

#include <vector>
#include <string>


class StrUtils {
public:
	static std::vector<std::string> explode(const std::string& s, const char& c);
	static bool startsWith(const char *s, const char *prefix);

};

#endif /* CORE_STRUTILS_H_ */
