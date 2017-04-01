/* CS232_proj1_SystemCalls 
Shurjo Maitra
sm47
Febuary 17, 2017 */


#include <stdio.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

//must use pause(), alarm(sec), signal(sig, handler)

char *message = "Warning! Virus Detected!"; //annoying message displayed

void signal_handle(int sig_num) {  //handles signal input, displays prompt
	printf("\n Program can't be terminated using Ctrl+C! LOL! \n");
}

void signal_alarm(int sig_num){ //assigns message to output when alarm is triggered
	printf("%s\n", message);
	fflush(stdout);
}

int main (int argc, char *argv[])
{
	bool interrupt = false; //by default fase
	int time = 5; //default time interval between alarms
	int i; //indexes through the argument vector

	if (argc > 1) //if there is more than 1 argument in the vector
	{
		for (i = 1; i < argc; i++)
		{
			if (strcmp(argv[i], "-i") == 0) { //if string matches vector index
				interrupt = false;
			}
			else if (strcmp(argv[i], "+i") == 0) {
				interrupt = true;
			}
			else if (strcmp(argv[i], "+t") == 0) {
				time = atoi(argv[i + 1]);
			}
			else if (strcmp(argv[i], "-m") == 0) {
				message = argv[i + 1];
			}
		}
		if (!interrupt) { //handles the mode when interrupts are disabled
			signal(SIGINT, signal_handle);
		}
		signal(SIGALRM, signal_alarm); //triggers signal

		for(;;) { 	//infinite loop
			alarm(time);
			pause();
		}
	}
	else	// when not enough arguments are in the input
	{
		printf("The program needs inputs!\n");
	}

	return 0;
}
