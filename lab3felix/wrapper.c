#include "wrapper.h"

#define MAX_SIZE 1024

struct mq_attr attr;

int MQcreate (mqd_t * mq, char * name)
{
    /* Creates a mailslot with the specified name. Uses mq as reference pointer, so that you can 	reach the handle from anywhere */
    /* Should be able to handle a messages of any size */
    /* Should return 1 on success and 0 on fail*/
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;
    *mq = mq_open(name, O_CREAT | O_NONBLOCK | O_RDWR, 0666, &attr);
    if(*mq == -1){
        return 0;
    }
    else{
        return 1;
    }

}
int MQconnect (mqd_t * mq, char * name)
{
    /* Connects to an existing mailslot for writing Uses mq as reference pointer, so that you can 	reach the handle from anywhere*/
    /* Should return 1 on success and 0 on fail*/
    int retVal;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;
    *mq = mq_open(name, O_RDWR, 0666, &attr);
    if(*mq == -1)
        return 0;
    else
        return 1;
}

int MQread (mqd_t * mq, void ** refBuffer)
{
    /* Read a msg from a mailslot, return nr Uses mq as reference pointer, so that you can 		reach the handle from anywhere */
    /* of successful bytes read              */
    int retVal;
    retVal = (int) mq_receive(*mq, (char *)refBuffer, MAX_SIZE + 1, NULL);
    if(retVal == -1){
        retVal = 0;
    }
    return retVal;
}

int MQwrite (mqd_t * mq, void * sendBuffer)
{
    /* Write a msg to a mailslot, return nr Uses mq as reference pointer, so that you can 	     reach the handle from anywhere*/
    /* of successful bytes written         */
    int retVal;
    retVal = mq_send(*mq, (const char *) sendBuffer, sizeof(planet_type), 0);
    if(retVal == -1){
        retVal = 0;
    }
    return retVal;
}

int MQclose(mqd_t * mq, char * name)
{
    /* close a mailslot, returning whatever the service call returns Uses mq as reference pointer, so that you can 	
    reach the handle from anywhere*/
    /* should return 1 on success and 0 on fail*/
    int retVal;
    retVal = mq_close(*mq);
    retVal += mq_unlink(name);
    if(retVal == 0){return 1;}
    else{return 0;}
}
int threadCreate (void * functionCall, int threadParam)
{
	/* Creates a thread running threadFunc */
	/* Should return 1 on success and 0 on fail*/
    pthread_t TID;
    if (pthread_create(&TID, NULL, functionCall, &threadParam) != 0){
        return 0;
    }
    else{
        return 1;
    }
}



