#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/syscall.h>

// A normal C function that is executed as a thread when its name
// is specified in pthread_create()
sem_t customerSemaphore;
sem_t bakerSemaphore;
sem_t storeSemaphore;
pthread_t tid[10];
int customers = 0;
int breadBaked = 0;
bool customerWaiting = false;
int breadAvailable = 0;

int getThreadNum() {
    pthread_t id = pthread_self();
    for(int i = 0; i < 10; i++) {
        if (pthread_equal(id, tid[i])) {
            return i;
        }
    }

    return -1;
}

void bakerCheckout() {
    printf("Customer [%d] waiting to checkout...\n", getThreadNum());
    sem_wait(&bakerSemaphore);
    //CRITICAL SECTION
    sleep(1);
    printf("Customer [%d] has just checked out!\n", getThreadNum());
    sem_post(&bakerSemaphore);
}

void *customerActions(void *vargp)
{
    printf("Customer [%d] attempting to enter the store...\n", getThreadNum());
    sem_wait(&storeSemaphore);
    printf("Customer [%d] entered the store!\n", getThreadNum());

    sem_wait(&customerSemaphore);
    //request bread
    printf("Customer [%d] waiting for bread...\n", getThreadNum());
    while(breadAvailable == 0) {
      	sleep(0.1); //sleep, check again
    }

    breadAvailable--;
    printf("Customer [%d] received bread!\n", getThreadNum());
    sleep(2);
    //pay
    bakerCheckout();
    //Finish the crtical section
    customers--;
    sem_post(&customerSemaphore);
    sem_post(&storeSemaphore);
    return NULL;
}

void *bakeBread() {
    printf("Bread baker is ready to work!\n");
    while(breadBaked != 10) {
        sem_wait(&bakerSemaphore);
        //CRITICAL SECTION
        breadAvailable++;
	      breadBaked++;
        printf("Baker just baked a single loaf of bread\n");
        sem_post(&bakerSemaphore);
        sleep(1);
    }
    printf("Baker is completely done baking loaves of bread.\n");
}

void initSemaphores() {
    sem_init(&customerSemaphore, 0, 1);
    sem_init(&bakerSemaphore, 0, 1);
    sem_init(&storeSemaphore, 0, 5);
}

int main()
{
    initSemaphores();
    printf("---Creating Threads---\n");
    pthread_t bakeBreadId;
    pthread_create(&bakeBreadId, NULL, bakeBread, NULL);
    for(int i = 0; i < 10; i++) {
        customers++;
        pthread_create(&tid[i], NULL, customerActions, NULL);
    }
    pthread_exit(NULL);
    printf("---All customers have finished---\n");
    exit(0);

/*
 * bakeshop.c simulates a bread shop with a baker
 *
 */

#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

sem_t bakerSemaphore;
sem_t customerSemaphore;
sem_t storeSemaphore;
int loavesBaked = 0;
int loavesAvailable = 0;

void checkoutActions(long id_num) {

    fprintf(stderr, "Customer %ld is waiting to check out.\n", id_num);
    sem_wait(&bakerSemaphore);
    usleep(1000000);
    fprintf(stderr, "Customer %ld just checked out!\n", id_num);
    sem_post(&bakerSemaphore);

}

void* bakeBreadActions() {

    fprintf(stderr, "Store is open and baker is ready to bake!\n");
    while (loavesBaked < 10) {
        sem_wait(&bakerSemaphore);
        loavesAvailable += 1;
        loavesBaked += 1;
        fprintf(stderr, "Baker just baked loaf number %d.\n", loavesBaked);
        sem_post(&bakerSemaphore);
        usleep(1000000);
    }

    fprintf(stderr, "All bread has been baked!\n");

}

void* customerActions(void *id_num) {

    long id;
    id = (long)id_num;

    fprintf(stderr, "Customer %ld is waiting to get into the store.\n", id);
    sem_wait(&storeSemaphore);
    fprintf(stderr, "Customer %ld got into the store.\n", id);

    sem_wait(&customerSemaphore);
    while (loavesAvailable == 0) {
        usleep(1000000);
    }
    fprintf(stderr, "Customer %ld has a loaf of bread.\n", id);
    loavesAvailable -= 1;
    usleep(1000000);

    checkoutActions(id);

    fprintf(stderr, "Customer %ld has left the store.\n", id);
    sem_post(&customerSemaphore);
    sem_post(&storeSemaphore);

}

 int main(int argc, char *argv[]) {

    sem_init(&bakerSemaphore, 0, 1);
    sem_init(&customerSemaphore, 0, 1);
    sem_init(&storeSemaphore, 0, 5);

    pthread_t threads[10];
    int bakerReturnValue;
    int customerReturnValue;

    pthread_t bakerThread;
    bakerReturnValue = pthread_create(&bakerThread, NULL, bakeBreadActions, NULL);

    if (bakerReturnValue) {
        fprintf(stderr, "ERROR; return code from pthread_create() is %d\n", bakerReturnValue);
        exit(-1);
    }

    for (long i = 0; i < 10; i++) {
        customerReturnValue = pthread_create(&threads[i], NULL, customerActions, (void *)i);
        if (customerReturnValue){
            fprintf(stderr, "ERROR; return code from pthread_create() is %d\n", customerReturnValue);
            exit(-1);
        }
    }

    fprintf(stderr, "Bakery is now closed for the evening!");

    pthread_exit(NULL);

 }
}