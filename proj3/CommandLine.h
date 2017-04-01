/*
 * CommandLine.h class to read a command-line
 *
 * Written by: Frank Boye
 * CS 232 - Project 3
 * March 6, 2017
 */

#ifndef COMMANDLINE_H_
#define COMMANDLINE_H_

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sstream>

using namespace std;

//class to read a command-line,
class CommandLine {

//public functions
public:
	CommandLine();
	CommandLine(istream& in);
	char* getCommand() const;
	int getArgCount() const;
	char** getArgVector() const;
	char* getArgVector(int i) const;
	bool noAmpersand() const;
	virtual ~CommandLine();

//private instance variables
private:
	int argc;
	char** argv;
	vector<string> myArray;
};

#endif /* COMMANDLINE_H_ */
