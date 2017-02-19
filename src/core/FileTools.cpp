/*
 * FileTools.cpp
 *
 *  Created on: May 7, 2016
 *      Author: adrien
 */


#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>          // need for tostr() function
#include <cstdio>           // need for popen() function in executeShellCmd()
#include <dirent.h>         // need for dirent struct in searchDirectory()
#include <iostream>
#include "FileTools.h"

using namespace std;

vector<string> FileTools::readLinesInFile(string pFilepath)
{
	string line;
	vector<string> results;
	ifstream myfile (pFilepath);
	if (myfile.is_open())
	{
		while ( getline (myfile,line) )
		{
			results.push_back(line);
		}
		myfile.close();
	}

	return results;
}

bool FileTools::writeToFile(string pFilepath, string pContent)
{
	ofstream enableFile;
	enableFile.open(pFilepath.c_str());
	enableFile << pContent.c_str();
	enableFile.close();

	return true;
}

string FileTools::searchDirectory(string searchPath, string searchPattern)
{
	string str;
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

void FileTools::appendToFile(std::vector<std::string> pMessages, std::string pFilepath)
{
	// TODO to be implemented
}

FileTools::FileTools() {
	// TODO Auto-generated constructor stub

}

FileTools::~FileTools() {
	// TODO Auto-generated destructor stub
}

