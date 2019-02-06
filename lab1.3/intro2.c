#include "wrapper.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <mqueue.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include "wrapper.c"

#define QUEUE_NAME "/mq3" //name "MQ" does not work
#define MAX_SIZE 1024

pthread_mutex_t l1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t l2 = PTHREAD_MUTEX_INITIALIZER;

pthread_t prod;

void * producer(void * u) {
    printf("Starting mail client..\n");

    mqd_t mq;
    planet_type msg;
    //mq = mq_open(QUEUE_NAME, O_CREAT | O_NONBLOCK | O_RDONLY, 0666, &attr);
    if (MQcreate(&mq, QUEUE_NAME) != 1){
        printf("ERROR!\n");
        return NULL;}
    printf("mq_created %d\n", mq);
    pthread_mutex_unlock(&l1);
    while (1) {
        //printf("mutex_prod lock: %d\n",pthread_mutex_lock(&l2));
        pthread_mutex_lock(&l2);
        if (MQread(&mq, (void *)&msg) >= 0){
            if(strcmp(msg.name, "END")==0){
                mq_close(mq);
                return 0;
            }
            printf("%s\n", msg.name);
        }
        //printf("mutex_prod unlock: %d\n",pthread_mutex_unlock(&l1));
        pthread_mutex_unlock(&l1);
        usleep(20);
    }
}

int main(){
    printf("Creating threads...\n");
    pthread_mutex_init(&l1, NULL);
    pthread_mutex_init(&l2, NULL);

    pthread_mutex_lock(&l1);
    pthread_mutex_lock(&l2);
    pthread_create(&prod, NULL, &producer, NULL);
    mqd_t mq;
    planet_type msg;
    //open mq
    //usleep(20);
    pthread_mutex_lock(&l1);
    if (MQconnect(&mq, QUEUE_NAME) != 1){
        printf("ERROR!\n");
        return -1;
    }
    pthread_mutex_unlock(&l1);
    while(1){
        //printf("mutex_main lock: %d\n",pthread_mutex_lock(&l1));
        pthread_mutex_lock(&l1);
        scanf("%s", msg.name);
        //printf("mq_send: %d\n", mq_send(mq, (const char*)&input, sizeof(input), 0));
        MQwrite(&mq, (void *)&msg);
        //printf("mutex_main unlock: %d\n", pthread_mutex_unlock(&l2));
        pthread_mutex_unlock(&l2);
        usleep(20);
        if(strcmp(msg.name, "END") == 0){
            int retVal = MQclose(&mq, QUEUE_NAME);
            if(retVal == 1){
                printf("Mailslot closed successfully...\n");
            }
            else
                printf("Mailslot NOT closed successfully!!\n");
            return 0;
        }
    }
}
