#include "wrapper.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t lock;
pthread_t TID;

void * hellomoon(void* u){
    while(1){
        pthread_mutex_lock(&lock);
        for(int k = 0; k < 10; k++){
            printf("Hello moon!\n");
            usleep(200000);
        }
        pthread_mutex_unlock(&lock);
        usleep(1000);
    }
}

int main(){
    if(pthread_mutex_init(&lock, NULL) != 0){
        printf("Mutex could not be created!\n");
        return 0;
    }
    pthread_create(&TID, NULL, &hellomoon, NULL);
    while(1){
        if(pthread_mutex_lock(&lock)!=0){
            printf("QWEQWEQWE\n");
        }
        for(int i = 0; i < 10; i++){
            printf("Hello World!\n");
            sleep(1);
        }
        pthread_mutex_unlock(&lock);
        usleep(1000);
    }

}
