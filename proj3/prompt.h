/*
 * prompt.h provides a class to represent the command-line promp
 *
 * Written by: Shurjo Maitra
 * CS 232 - Project 3
 * Partner: Frank Boye
 * March 6, 2017
 */

#ifndef PROMPT_H_
#define PROMPT_H_

// Includes
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <limits.h>
#include <unistd.h>
using namespace std;

/**
 *Keeps and manipulates cwd
 */

// Class Declaration
class Prompt {

// Method declarations
public:
	Prompt();
	char* get() const;

// Private inst vars
private:
	char* cwd;
	char buff[PATH_MAX + 1];
};
#endif