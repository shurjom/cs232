/*
 * FSShell.cpp implements the customized shell program
 *
 * Written by: Shurjo Maitra and Frank Boye
 * CS 232 - Project 3
 * March 13, 2017
 */

// Include FSShell.h header file
#include "FSShell.h"
using namespace std;

void FSShell::run() {
	//loop until exit
	while (true) {
		//print out the current working direcotry (prompt dir)
		cout << pr.get() << "$ ";
		CommandLine cl = CommandLine(cin);

		//exits/stops program when exit is read in cmd line
		if (strcmp(cl.getCd(), "exit") == 0) {
			cout << "exit" << endl;
			return;
		}

		//changes dir when cd is detected otherwise throws error
		if (strcmp(cl.getCd(), "cd") == 0) {
			int ret_val = chdir(cl.getArgVector(1));
			if (ret_val == -1) {
				cout << "invalid dir" << endl;
			} else {
				pr = pr();
			}
			continue;
		}

		//prints out current working dir when pwd is detected
		if (strcmp(cl.getCd(), "pwd") == 0) {
			cout << endl << pr.get() << endl;
			continue;
		}

		//when cmd is not in predefined list throw errors for different instances
		if (pa.find(cl.getCd()) == -1) {
			cout << "command not found" << endl;
		} 
		//if cmd is valid create a child process
		else {
			//get the process id and assigns to fork
			pid_t child_proc = fork();
			//throw error if previous cmd index is tried
			if (child_proc == -1) {
				cout << "fork failed" << endl;
			}
			// process initialization 
			if (child_proc == 0) {
				//get index of path of cmd
				int index = pa.find(cl.getCd());
				//get directory path of cmd
				char* temp_char = pa.getDir(index);
				strcat(temp_char, "/");
				//append cmd to end of path dir
				strcat(temp_char, cl.getCd());
				//get arg vec and append NULL to end
				char ** temp_vec = cl.getArgVector();
				temp_vec[cl.getArgCount()] = NULL;
				//execute cmd with execve()
				execve(temp_char, temp_vec, NULL);

			}

			int status;
			//If ampersand is not in input wait for child process to finish
			if (cl.noAmpersand()) {
				waitpid(child_proc, &status, 0);
			}

			cout << endl;
		}
	}

}