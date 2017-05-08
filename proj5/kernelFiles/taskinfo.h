struct taskinfo {
 	pid_t pid;                  /* process id */
 	pid_t parent_pid;           /* process id of parent */
 	pid_t first_child_pid;      /* pid of youngest child */
 	long state;                 /* current state of process */
 	long uid;                   /* user id of process owner */
 	char comm[64];              /* name of program executed */ 
 	int level;                  /* the output indentation level */
};