#include "wrapper.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mymutex=PTHREAD_MUTEX_INITIALIZER;
int turn = 1;

void * hello_moon(void* u){
    while(1) {
        if(turn == 0) {
            pthread_mutex_lock(&mymutex);
            for (int k = 0; k < 10; k++) {
                printf("%d %s", k, (char *) u);
                usleep(300000);
            }
            pthread_mutex_unlock(&mymutex);
            turn = 1;
        }
    }
}

void * hello_world(void* u){
    while(1){
        if(turn == 1){
            pthread_mutex_lock(&mymutex);
            for (int k = 0; k < 10; k++){
                printf("%d %s", k, (char*) u);
                usleep(300000);
            }
            pthread_mutex_unlock(&mymutex);
            turn = 0;
        }
    }
}

int main(){
    pthread_t tid_world;
    pthread_t tid_moon;

    char * hellomoon = "Hello Moon!\n";
    char * helloworld = "Hello World!\n";

    pthread_mutex_init(&mymutex,0);

    pthread_create(&tid_world, NULL, &hello_world, helloworld); //create both threads
    pthread_create(&tid_moon, NULL, &hello_moon, hellomoon);

    pthread_join(tid_world, NULL); //wait for both threads to terminate before exiting program
    pthread_join(tid_moon, NULL);
    }

