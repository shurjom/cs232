/*
 *Created by: Shurjo Maitra (sm47)
 *Date: 4/27/17
 *Runs tasktree system call in kernel
 */
#include <tasktree.h>
#include <taskinfo.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	// Structure taskinfo is put put into process pointer
	struct taskinfo *processes;
		
	//Number of initial processes to analyze 
	int nr = 100;
	
	//Allocate memory 
	processes = malloc(nr*sizeof(struct taskinfo));
			
	//r
	int num = syscall(327, processes, nr, 2);
		
	printf("Number of related processes found: %i\n", num);

	//Initialize loop variables 
	int i;
	int j;
	//Iterate through nr number of processes and find taskinfo details and output them
	for ( i = 0; i < nr; i++ )
	{  
		for( j = 0; j < processes[i].level; j++ ) {
			printf( "\t" ) ; 
		}
		printf( "pid %i, parent_pid %i, first_child %i, state %lu, uid %lu name %s\n", processes[i].pid, processes[i].parent_pid, processes[i].first_child_pid, processes[i].state, processes[i].uid, processes[i].comm);
	}
	//Release memory held by process malloc 
	free(processes);
	return 0;
}
