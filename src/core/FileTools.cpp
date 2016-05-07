/*
 * FileTools.cpp
 *
 *  Created on: May 7, 2016
 *      Author: adrien
 */

#include <cstring>
#include <string>
#include <sstream>          // need for tostr() function
#include <cstdio>           // need for popen() function in executeShellCmd()
#include <dirent.h>         // need for dirent struct in searchDirectory()
#include "FileTools.h"



std::string FileTools::searchDirectory(std::string searchPath, std::string searchPattern)
{
	std::string str;
	DIR *path;
	dirent *entry;

	path = opendir(searchPath.c_str());

	if( path != NULL  )
	{
		while( (entry = readdir(path)) != NULL)
		{
			if( entry->d_name[0] == '.')
			{
				continue;
			}

			str = entry->d_name;
			if(strstr(entry->d_name, searchPattern.c_str()) != NULL )
			{
				closedir(path);
				return str;
			}
		}
	}

	closedir(path);
	return "";
}

FileTools::FileTools() {
	// TODO Auto-generated constructor stub

}

FileTools::~FileTools() {
	// TODO Auto-generated destructor stub
}

