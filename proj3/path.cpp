/*
 * path.cpp  is a class to represent the dir 
 *           in which your shell searches for executable programs
 *
 * Written by: Shurjo Maitra
 * CS 232 - Project 3
 * Partner: Frank Boye
 * March 6, 2017
 */

//Header file path.h
#include "path.h"
using namespace std;

/*Path constructor
*@get: path environment variable 
*@store: each directory in PATH
*@method: split string token by symbol ':'
*/
Path::Path() {
	pPath = getenv("PATH");
	char* pch;
	pch = strtok (pPath, ":");

	while (pch != NULL) {
		dir.push_back(pch);
		pch = strtok (NULL, ":");
	}
}

/*
*@find: (const string& program) 
*@brief:locates dir in which program is stored
*@method: start at index -1, iterate thourgh dir size 
*		  if program is found assign it to ent otherwise close dir
*/
int Path::find(const string& program) const {
	int index = -1;
	DIR = *dir;
	struct dirent *ent;
	for (unsigned i = 0; i < dir.size(); i++) {
		if ((dir = opendir( dir[i])) != NULL) {
			if (ent -> d_name == program) {
				index = i;
			}
		}
		closedir(dir);
	}
	return index;
}

/*getDir method
*@return: directory found at vec index i
*@method: compare index with dir size, if greater throw error
*		  otherwise return temperary directory
*/
char* Path::getDir(int i ) const {
	if (i > dir.size()) {
		throw range_error("Index invalid");
	}
	char* temp_dir - dir[i];
	return temp_dir;
}