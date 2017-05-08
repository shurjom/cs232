/* Written by: Shurjo Maitra (sm47)
 * Date: April 28, 2017
 * Using printk, print out team name to kernel log
 */
SYSCALL_DEFINE0(printteamname)
{
	printk("Shurjo");
	return 0;
}

/* Written by: Shurjo Maitra (sm47)
 * Date: April 28, 2017
 * Gets process info and then recurseively iterates through its child 
 * processes. 
 * @param: task struct root_ptr, task array, int level
 * @return: process count, branches in tree
 */

int do_tasktree( struct task_struct * root_ptr, struct taskinfo __user * task_array, int nr, int level ) {
	
	//Declare structures to pointers below
 	struct list_head * list;
 	struct task_struct * child;

	//Return null if process number less than 
 	if ( nr <= 0 ) {
    	return 0;
 	}

	//Find all the branches of task tree and assign them to pid
	task_array[0].pid = root_ptr->pid; 
	//Parent process id
	task_array[0].parent_pid = root_ptr->parent->pid; 
	//state of process from task 
	task_array[0].state = root_ptr->state;		
	//level or process from task
  	task_array[0].level = level;		
  	//initiaize first child as 0 index
 	task_array[0].first_child_pid = 0;
 	//user id from task array to value
 	task_array[0].uid = root_ptr->cred->uid.val;		

	//Find command size or default to max value of 63
	int comm_size;
	if (sizeof(root_ptr->comm) > 63) {
		comm_size = sizeof(root_ptr -> comm);
	} 
	else {
		comm_size = 63;
	}
	
	//Itereate through main command and copy to taskinfo
	int i;
  	for( i = 0; i < comm_size; i++ ){
		task_array[0].comm[i] = root_ptr->comm[i];  
	}
	
	//Initialize loop variables
	int child_count = 1;
	int tmp;
  
	//Iterate trough branch children processes 
  	list_for_each(list, &root_ptr->children) {
    		child = list_entry(list, struct task_struct, sibling);

		//First child process is detected
   	 	if(child_count==1){
      		task_array[0].first_child_pid = child->pid;
    	}

		//Iterate through each child process recursively
    	tmp = do_tasktree(child, &task_array[child_count], nr - child_count, level + 1);
		
		//If there are no more child processes stop the loop
		if(tmp == 0){
      		break;
    	}
		//Increment child_count with number of descendants already done
    	child_count += tmp;
  	}
	return child_count;
}

/* Written by: Shurjo Maitra (sm47)
 * Date: April 28, 2017
 * Initialize variables, check for bad args and then call do_tasktree function
 * @param: task info, info buf, int nr, int rootpid
 * @return: related process count 
 */

SYSCALL_DEFINE3( tasktree, struct taskinfo __user *, buf, int, nr, int, rootpid){
	
	//Initialize starting task structure and array
	struct task_struct * root;
	struct taskinfo * task_array;
	
	//Check if buffer is empty, otherwise declare parameter check
  	if(buf == NULL || nr < 0){
		return -EINVAL;
	} else {
		printk("Function tasktree() passed parameter checks");
	}
	
	//Lock read access to task struct
	read_lock(&tasklist_lock);

	//Find the task structure using the root pid
	root = find_task_by_vpid(rootpid);

	//Allocate memory for array of size equal to taskinfo
	task_array = kmalloc( nr * sizeof( struct taskinfo), GFP_KERNEL );

	//Initialized count and iterates through processes recursively,
	// if struct is writable then write out the buffer otherwise return error
	int process_count =  do_tasktree( root, task_array, nr, 0 );
	if (!access_ok(VERIFY_WRITE, buf, sizeof(struct taskinfo))){
		 return -EFAULT;
	}

	//Copy buffer to the task array of size from taskinfo
 	copy_to_user( buf, task_array, nr * sizeof( struct taskinfo) );

	//Unlock read access to the task struct
	read_unlock(&tasklist_lock); 
	
	//Return task array space to kernel and output the process count
	kfree(task_array);
	return process_count;
}
