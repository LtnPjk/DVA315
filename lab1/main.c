#include "wrapper.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <mqueue.h>
#include <string.h>
#include <errno.h>

#define QUEUE_NAME "/mq"
#define MAX_SIZE    1024
#define STOP       "END"

pthread_mutex_t lock;
pthread_t prod;

void * producer(void * u){
    mqd_t mq;
    struct mq_attr attr;
    char msg[MAX_SIZE+1];

    //Init queue attr
    attr.mq_flags = 0;
    attr.mq_maxmsg = 100;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    //Create mq
    printf("Starting mail client..\n");
    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0644, &attr);
    while(1){
        mq_receive(mq, msg, MAX_SIZE, NULL);
        printf("%s\n", msg);
        if(strcmp(msg, "END\n") == 0){
            return 0;
        }
    }
}

int main(int ac, char * argv)
{
	printf("Creating threads...\n");
    pthread_create(&prod, NULL, &producer, NULL);
    mqd_t mq;

    //open mq
    mq = mq_open(QUEUE_NAME, O_WRONLY);

    char inp[MAX_SIZE];
    char *inpAddr = inp;

    while(1){
        scanf("%s\n", inp);
        //mq_send(mq, inp, MAX_SIZE, 0);
        if(strcmp(inp, "END") == 0){
            return 0;
        }
    }


}

