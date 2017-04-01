/*
 * prompt.cpp provides a class to represent the command-line promp
 *
 * Written by: Shurjo Maitra
 * CS 232 - Project 3
 * Partner: Frank Boye
 * March 6, 2017
 */

// Include prompt class header file
#include "prompt.h"
using namespace std;

/*
 * constructor method prompt()
 * @brief: gets cwd (current working dir)
 * @method: finds the current working directory by scanning buffer
 */
Prompt::Prompt() {
	cwd = getcwd(buff, PATH_MAX + 1); 
	strcat(cwd, "/FSShell");
}

/*
 * accessor method get()
 * @return: cwd variable
 * @method: retuns cwd
 */
char* Prompt::get() const {
	return cwd;
}