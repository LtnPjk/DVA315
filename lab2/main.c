typedef int buffer_item;
#define BUFFER_SIZE 5

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define RAND_DIVISOR 100000000
#define TRUE 1

pthread_mutex_t mutex;

sem_t full, empty;

/* the buffer */
buffer_item buffer[BUFFER_SIZE]={1,2,3,4,5};
int counter;
int ret;
int count=6;

pthread_t tid;       //Thread ID
pthread_attr_t attr; //Set of thread attributes

void *producer(void *param); /* the producer thread */
void *consumer(void *param); /* the consumer thread */
int remove_item(buffer_item *item);
int insert_item(buffer_item item);

void printBuffer(){
    for(int i = 0; i < BUFFER_SIZE; i++){
        printf("%d  ", buffer[i]);
    }
    printf("\n");
}

void initializeData() {

    /* Create the mutex lock */
    pthread_mutex_init(&mutex, NULL);

    /* Create the full semaphore and initialize to 0 */
    ret = sem_init(&full, 0, 0);

    /* Create the empty semaphore and initialize to BUFFER_SIZE */
    ret = sem_init(&empty, 0, BUFFER_SIZE);

    /* Get the default attributes */
    pthread_attr_init(&attr);

    /* init buffer */
    counter = 0;
}

/* Producer Thread */
void *producer(void *param) {
    buffer_item item;

    printf("Producer created!\n");
    fflush(stdout);
    while(TRUE) {
        /* sleep for a random period of time */
        int rNum = rand() / RAND_DIVISOR;
        sleep(rNum);

        item = count++;

        sem_wait(&empty);

        if(insert_item(item)) {
            fprintf(stderr, " Producer %ld report error condition\n", (long) param);
	    fflush(stdout);
        }

        else {
            printf("producer %ld produced %d\n", (long) param, item);
        }
        printBuffer();
        sem_post(&full);
	fflush(stdout);
    }
}

/* Consumer Thread */
void *consumer(void *param) {
    buffer_item item;
    while(TRUE) {
        /* sleep for a random period of time */
        int rNum = rand() / RAND_DIVISOR;
        sleep(rNum);

        sem_wait(&full);

        if(remove_item(&item)) {
            fprintf(stderr, "Consumer %ld report error condition\n",(long) param);
	    fflush(stdout);
        }

        else {
            printf("consumer %ld consumed %d\n", (long) param, item);
        }
        printBuffer();
        sem_post(&empty);
    }
}

/* Add an item to the buffer */
int insert_item(buffer_item item) {
    /* When the buffer is not full add the item
     and increment the counter*/

    pthread_mutex_lock(&mutex);
    if(counter < BUFFER_SIZE) {
        buffer[counter] = item;
        counter++;
        pthread_mutex_unlock(&mutex);
        return 0;
    }
    else { /* Error the buffer is full */
        pthread_mutex_unlock(&mutex);
        return -1;
    }
}

/* Remove an item from the buffer */
int remove_item(buffer_item *item) {
    /* When the buffer is not empty remove the item
     and decrement the counter */
    pthread_mutex_lock(&mutex);
    if(counter > 0) {
        *item = buffer[(counter-1)];
        counter--;
        pthread_mutex_unlock(&mutex);
        return 0;
    }
    else { /* Error buffer empty */
        pthread_mutex_unlock(&mutex);
        return -1;
    }
}

int main(int argc, char *argv[]) {
    long i;
    void * status;
    /* Verify the correct number of arguments were passed in */
    //if(argc != 4) {
    //    fprintf(stderr, "USAGE:./main.out <INT> <INT> <INT>\n");
    //}

    int mainSleepTime = 30;//atoi(argv[1]); /* Time in seconds for main to sleep */
    int numProd = 5; //atoi(argv[2]); /* Number of producer threads */
    int numCons = 8; //atoi(argv[3]); /* Number of consumer threads */

    initializeData();

    /* Create the producer threads */
    for(i = 0; i < numProd; i++) {
        pthread_create(&tid,&attr,producer,(void*) i);
    }

    /* Create the consumer threads */
    for(i = 0; i < numCons; i++) {
        pthread_create(&tid,&attr,consumer, (void*) i);
    }
    pthread_join(tid, &status);
    /* Sleep for the specified amount of time in milliseconds */
    sleep(mainSleepTime);

    /* Exit the program */
    printf("Exit the program\n");
    exit(0);
}
