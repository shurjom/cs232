/*
 * path.h is a class that represents the directories in which our shell program searches
 *		for executable programs
 *
 * Written by: Shurjo Maitra
 * CS 232 - Project 3
 * Partner: Frank Boye
 * March 6, 2017
 */

#ifndef PATH_H_
#define PATH_H_

// Includes
#include <string>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <stdexcept>
#include <dirent.h>
using namespace std;

// Class declaration
class Path {

// Method declaration
public:
	Path();
	int find(const string& program) const;
	char* getDir(int i) const;

// Private variables
private:
	char* pPath;
	vector<char*> dir;
};

#endif
