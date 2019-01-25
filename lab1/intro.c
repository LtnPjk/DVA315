#include "wrapper.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void * hellomoon(void * u){
    while(1){
    printf("Hello moon!\n");
    usleep(200000);
    }
}

int main(){
    pthread_t tid;
    pthread_create(&tid, NULL, hellomoon, NULL);
    for(int i = 0; i < 10; i++){
        printf("Hello world!\n");
        sleep(1);
    }
}
