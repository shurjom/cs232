/*
 * CommandLine.cpp
 *
 * Written by: Frank Boye
 * CS 232 - Project 3
 * March 6, 2017
 */

/*
 * default constructor
 */
CommandLine::CommandLine() {
	argc = 0;
}

/*
 * explicit constructor
 * CommandLine(istream& in): construct a CommandLine by reading a command-line from in
 */
CommandLine::CommandLine(istream& in) {
	argc = 0;

	noAmpersand = true;

	string buffer;
	getline(in, buffer);
	istringstream sstream (buffer);

//Set noAmpersand to false if there is an ampersand in the command-line
	string tmp;
	while (sstream >> tmp) {
		if (strcmp(tmp.c_str(), "&") == 0) {
			noAmpersand = false;
		} else {
			myArray.push_back(tmp);
			argc++;
		}
	}

	argv = new char*[myArray.size()];

	for (size_t i = 0; i < myArray.size(); i++) {
		argv[i] = new char[myArray[i].size() + 1];
		strcpy(argv[i], myArray[i].c_str());
	}
}

/*
 * getCommand(): return a pointer to the command portion of the command-line
 */
char* CommandLine::getCommand() const{
	return argv[0];
}

/*
 * getArgCount(): return the number of command-line arguments on the command-line
 */
int CommandLine::getArgCount() const{
	return argc;
}

/*
 * getArgVector(): return a pointer to a char* array, each entry of which points to one 'word' of the command-line
 */
char** CommandLine::getArgVector() const{
	return argv;
}

/*
 * getArgVector(int i): return a pointer to the ith (zero-relative) command-line 'word'
 */
char* CommandLine::getArgVector(int i) const{
	return argv[i];
}

/*
 * noAmpersand(): returns true if and only if no ampersand was given on the command-line
 */
bool CommandLine::noAmpersand() const{
	return noAmpersand;
}

/*
 * ~CommandLine()
 * Destructor for the CommandLine class
 */
CommandLine::~CommandLine() {
	for (size_t i = 0; i < myArray.size(); i++) {
		delete[] argv[i];
	}

	delete[] argv;

}
