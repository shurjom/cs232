/*
 * bakeshop.c
 *
 * Written by: Shurjo Maitra
 * CS 232 - Project 4
 * April 5, 2017
 */

// Declare libraries
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/syscall.h>

//Declare semaphores
sem_t bakerSem;
sem_t customerSem;
sem_t storeSem;

//Declare global counting variables 
int bakedLoaves = 0;
int availableLoaves = 0;
int soldLoaves = 0;
int checkedoutCustomers = 0;
long threadCheckoutReady;


 /*
* bake()
* @function: to implement bake thread
* @method: make ten loaves of bread and announce when all are done
*/
 void* bake() {
 	fprintf(stderr, "\nStore open, oven hot, baker read to bake!\n\n" );
 	//while the number of baked loaves is less than 10
 	while (bakedLoaves < 10) {
 		//catch baker semaphore
 		sem_wait(&bakerSem);
 		//bake a loaf of bread and increment total count
 		availableLoaves += 1;
 		bakedLoaves += 1;
 		//announce the baked loaf of bread
 		fprintf(stderr, "Load %d is baked and ready to be sold!\n\n", bakedLoaves);
 		//release the baker semaphore
 		sem_post(&bakerSem);
 		// one second delay
 		usleep(1000000);
 	}
 	fprintf(stderr, "All loaves of bread have been baked for today!\n\n" );
}

/*
* checkout()
* @function: to impletement checkout thread
* @method: keep checking out loaves till baker has run out and then announce
*/
void* checkout() {
	//whhile all ten customer have not been checked out yet
	while (checkedoutCustomers < 10) {
		//if more loaves have been sold than customers sold, keep selling
		if (soldLoaves > checkedoutCustomers) {
		//catch customer semaphore
		sem_wait(&customerSem);
		fprintf(stderr, "Customer %ld is wating to be checked out! \n\n",threadCheckoutReady );
		//catch baker semaphore
		sem_wait(&bakerSem);
		//delay of one second
		usleep(1000000);
		//annnounce when customer has been processed through each step of the sale
		fprintf(stderr, "Customer %ld has paid for the loaf.\n\n", threadCheckoutReady);
		fprintf(stderr, "Baker printed receipt for customer %ld. \n\n",threadCheckoutReady);
		fprintf(stderr, "Customer %ld received a receipt\n\n", threadCheckoutReady);
		//checkout a loaf of bread and increment total count
		checkedoutCustomers += 1;
		//release baker semaphore
		sem_post(&bakerSem);
		//release baker semaphore
		sem_post(&customerSem);
		}
	}
	fprintf(stderr, "All customers have checkedout and received receipts!\n\n" );
 }

 /*
 * customer()
 * @function: to implement customer thread
 */
 void* customer(void *cust_id) {
 	long id;
	id = (long)cust_id;
	//cutomer attempts to enter store
 	fprintf(stderr, "Customer %ld is waiting outside store.\n\n", id );
 	//catch store semaphore
 	sem_wait(&storeSem);
 	//customer is waiting for a loaf once entered
 	fprintf(stderr, "Customer %ld has entered the store!\n\n", id );
 	//catch customer semaphore
 	sem_wait(&customerSem);
 	//while there are still no loaves ready, delay
 	while (availableLoaves == 0 ) {
 		//one second delay
 		usleep(1000000);

 	}
 	fprintf(stderr, "Customer %ld has gotten a loaf of bread.\n\n", id );
 	//grab  customer esmaphore
 	sem_post(&customerSem);
 	//increment loaves related quantitiies
 	availableLoaves += 1;
 	soldLoaves += 1;
 	threadCheckoutReady = id;

 	//one seocnd delay
	usleep(1000000);
	//catch customer semaphore
	sem_wait(&customerSem);
	fprintf(stderr, "Customer %ld has left the store with their bread.\n\n", id );
	//release customer semaphore
	sem_post(&customerSem);
	//release customer semaphore
	sem_post(&storeSem);
 }


/*
* main() initializes three semaphores, creates baking, customer and checkout threads,
*		 waits till all threads complete then terminates program
*/

int main() {
	//initialize semaphores
	sem_init(&bakerSem, 0, 1);
	sem_init(&customerSem, 0, 1);
	sem_init(&storeSem, 0, 5);
	
	//create two threads for  baker and customer
	pthread_t bakeThread;
	pthread_t checkoutThread;

	//initialize array of 10 threads for customers
	pthread_t threads[10];

	//delcare error integer values
	int bakerReturnVal;
	int customerReturnVal;
	int checkoutReturnVal;


 	bakerReturnVal = pthread_create(&bakeThread, NULL, bake, NULL);

 	//if pthreads  could not be pushed to bakerThread
 	if (bakerReturnVal) {
 		fprintf(stderr, "ERROR; return code out of bakeThread pthread_create() is %d\n\n", bakerReturnVal);
 		exit(-1);
 	}
 	checkoutReturnVal = pthread_create(&checkoutThread, NULL, checkout, NULL);

 	//if pthreads could not be pushed to checkoutThread
 	if (checkoutReturnVal) {
 		fprintf(stderr, "ERROR: return code out of checkoutThread pthread_create() is %d\n\n", checkoutReturnVal);
 		exit(-1);
 	}
 	//changes last customer priority
 	long i;
 	for (i = 0; i < 10; i++)  {
 		customerReturnVal = pthread_create(&threads[i], NULL, customer, (void *)i);
 		//if pthreads could not be pushed to customer Thread
 		if (customerReturnVal){
       		fprintf(stderr, "ERROR; return code out of customerThread pthread_create() is %d\n\n", customerReturnVal);
       		exit(-1);
       	}
 	}
 	pthread_exit(NULL);
 }






