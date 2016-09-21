/*
 * FileTools.h
 *
 *  Created on: May 7, 2016
 *      Author: adrien
 */

#ifndef SRC_CORE_FILETOOLS_H_
#define SRC_CORE_FILETOOLS_H_

#include <string>
#include <vector>

class FileTools {
public:
	FileTools();
	virtual ~FileTools();

	static bool writeToFile(std::string pFilepath, std::string pContent);
	static std::string searchDirectory(std::string searchPath, std::string searchPattern);
	static std::vector<std::string> readLinesInFile(std::string pFilepath);

};

#endif /* SRC_CORE_FILETOOLS_H_ */
