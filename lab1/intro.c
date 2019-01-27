#include "wrapper.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void * hellomoon(void* u){
    for(int k = 0; k < 10; k++){
    printf("%s", (char*)u);
    usleep(200000);
    }
}

int main(){
    pthread_t tid;
    char * word = "Hello Moon!\n";
    while(1){
        for(int i = 0; i < 10; i++){
            printf("Hello World!\n");
            sleep(1);
        }
        pthread_create(&tid, NULL, &hellomoon, word);
        pthread_join(tid, NULL);
    }
}
