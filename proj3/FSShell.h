/*
 * FSShell.h is a class that simulates a shell program
 *
 * Written by: Shurjo Maitra and Frank Boye
 * CS 232 - Project 3
 * March 6, 2017
 */

#ifndef FSSHELL_H_
#define SHELL_H_

// Include
#include <iostream>
#include <sys/wait.h>
#include "path.h"
#include "prompt.h"
#include "CommandLine.h"
using namespace std;

// Declare class
class FSShell {

// Public 
public:
	FSShell();

//Run shell init
	void run();

// Private inst var
private:
	Prompt pr;
	Path pa;
};
#endif