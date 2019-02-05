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

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_t prod;

struct mq_attr attr;
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
    mqd_t mq;
    mq = mq_open(QUEUE_NAME, O_CREAT | O_NONBLOCK | O_RDONLY, 0666, &attr);
    printf("mq_open-creat %d\n", mq);
    struct inputs input_buffer;
    while (1) {
        printf("mutex_prod lock: %d\n",pthread_mutex_lock(&lock));
        if (mq_receive(mq, (char *)&input_buffer, MAX_SIZE + 1, NULL) >= 0){
            if(strcmp(input_buffer.string, "END")==0){
                mq_close(mq);
                return 0;
            }
            printf("%s\n", input_buffer.string);
        }
        printf("mutex_prod unlock: %d\n",pthread_mutex_unlock(&lock));
        usleep(20);
    }
}

int main(){
    printf("Creating threads...\n");
    pthread_mutex_init(&lock, NULL);
    pthread_create(&prod, NULL, &producer, NULL);
    mqd_t mq;
    struct inputs input;
    //open mq
    sleep(1);
    mq = mq_open(QUEUE_NAME, O_WRONLY, 0666, &attr);
    while(1){
        printf("mutex_main lock: %d\n",pthread_mutex_lock(&lock));
        scanf("%s", input.string);
        printf("mq_send: %d\n", mq_send(mq, (const char*)&input, sizeof(input), 0));
        printf("mutex_main unlock: %d\n", pthread_mutex_unlock(&lock));
        usleep(20);
        if(strcmp(input.string, "END") == 0){
            mq_close(mq);
            mq_unlink(QUEUE_NAME);
            return 0;
        }
    }
}
