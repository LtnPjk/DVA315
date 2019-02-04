#include "wrapper.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <mqueue.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

#define QUEUE_NAME "/mq" //name "MQ" does not work
#define MAX_SIZE 1024

pthread_mutex_t lock;
pthread_t prod;

static mqd_t mq;
static struct mq_attr attr;
struct inputs{
    char string[MAX_SIZE];
};

void * producer(void * u) {
    //Init queue attr
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    //Create mq
    printf("Starting mail client..\n");
    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, 0666, &attr);
    while (1) {
        struct inputs input_buffer;
        if (mq_receive(mq, (char *)&input_buffer, MAX_SIZE + 1, NULL) >= 0)
            printf("%s\n", input_buffer.string);
    }
}

int main(){
    printf("Creating threads...\n");
    pthread_create(&prod, NULL, &producer, NULL);
    mqd_t mq;
    struct inputs input;

    //open mq
    mq = mq_open(QUEUE_NAME,O_NONBLOCK | O_RDWR, 0666, &attr);
    while(1){
        scanf("%s", input.string);
        if(strcmp(input.string, "END") == 0)
            return 0;
        mq_send(mq, (const char*)&input, sizeof(input), 0);
    }
}

