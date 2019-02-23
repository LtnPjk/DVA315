#ifndef WRAPPER_H
#define WRAPPER_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <mqueue.h>
#include <pthread.h>
#include <semaphore.h>

// Mailslot handling:
extern int threadCreate (void * functionCall, int threadParam);

extern int MQcreate (mqd_t * mq, char * name);
extern int MQconnect (mqd_t * mq, char * name);
extern int MQread (mqd_t * mq, void ** refBuffer);
extern int MQwrite (mqd_t * mq, void * data);

//Custom garbage
int counter;
#define BUFFER_SIZE 10
#define QUEUE_NAME "/mq1"
#define SEM_EMPTY "/sem_empty14"
#define SEM_FULL "/sem_full14"
#define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)

// Struct for planet data will be used in lab 2 and 3 !!!!!
// Just ignore in lab1 or you can try to send it on your mailslot,
// will be done in lab 2 and 3

typedef struct vec{
    double x;
    double y;
}vector2D;

typedef struct pt {
	char		name[20];	// Name of planet
	double		sx;			// X-axis position
	double		sy;			// Y-axis position
	double		vx;			// X-axis velocity
	double		vy;			// Y-axis velocity
	double		mass;		// Planet mass
	struct pt*	next;		// Pointer to next planet in linked list
	int			life;		// Planet life
	char		pid[30];	// String containing ID of creating process
    //custom garbage
    vector2D    points[100];
    int         radius;
    double      mult;
} planet_type;

#endif /* WRAPPER_H */
