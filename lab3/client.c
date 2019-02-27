#include "wrapper.h"

mqd_t mq1;
mqd_t mq2;

void *death_server(char deathName[]){
    sem_t *sem_empty2 = sem_open(SEM_EMPTY2, O_CREAT, 0644, BUFFER_SIZE);
    sem_t *sem_full2 = sem_open(SEM_FULL2, O_CREAT, 0644, 0);
    sem_t *sem_mutex2 = sem_open(SEM_MUTEX2, O_CREAT, 0644, 1);

    if(MQcreate(&mq1, deathName) != 1){
        printf("Could not create mailslot!\n");
        return NULL;
    }
    /* printf("mailslot created!\n"); */
    deathInfo dInfo;
    while(1){
        sem_wait(sem_full2);
        sem_wait(sem_mutex2);
        if(MQread(&mq1, (void **) &dInfo) >= 1){
            printf("Planet %s died of %s\n", dInfo.name, dInfo.message);
        }
        sem_post(sem_mutex2);
        sem_post(sem_full2);
    }
}

int main()
{
    pthread_t deathServer;
    //Implement you inter process communication here, happy coding
    planet_type planet;

    sprintf(planet.pid, "/%d", (int)pthread_self());
    int ku = pthread_create(&deathServer, NULL, (void*(*)(void*)) death_server, planet.pid);

    planet_type P1;
    strcpy(P1.name, "P1");
    P1.mass = 100000000;
    P1.life = 1000000000;
    P1.sx = 300;
    P1.sy = 300;
    P1.vx = 0.000;
    P1.vy = 0.000;

    planet_type P2;
    strcpy(P2.name, "P2");
    P2.mass = 1000;
    P2.life = 1000000000;
    P2.sx = 200;
    P2.sx = 300;
    P2.vx = 0.000;
    P2.vy = 0.008;

    mqd_t mq;

    sem_t *sem_empty = sem_open(SEM_EMPTY, 0);
    sem_t *sem_full = sem_open(SEM_FULL, 0);
    sem_t *sem_mutex = sem_open(SEM_MUTEX, 0);
    int p;
    while(1){
        printf("---  Create Planet  ---\n");
        printf("Name:\tx-pos\ty-pos\tx-vel\ty-vel\tmass\tlife\n");
        scanf("%s",  planet.name);
        scanf("%lf", &planet.sx);
        scanf("%lf", &planet.sy);
        scanf("%lf", &planet.vx);
        scanf("%lf", &planet.vy);
        scanf("%lf", &planet.mass);
        scanf("%d", &planet.life);
        printf("Spawning planet...\n");

        if(MQconnect(&mq2, QUEUE_NAME) != 1){
            printf("CLIENT: Could not connect!\n");
        }
        else{
            sem_wait(sem_empty);
            sem_wait(sem_mutex);
            printf("CLIENT: Connected!\n");
            p = MQwrite(&mq2, (void *)&planet);
            /* if(garb == 1) */
            /*     MQwrite(&mq, (void*)&P1); */
            /* if(garb == 2) */
            /*     MQwrite(&mq, (void*)&P2); */
            printf("CLIENT: Request returned: %d\n", p);
            if(p==0){
                printf("ERROR: %d\n", errno);
            }
            sem_post(sem_mutex);
            sem_post(sem_full);
        }
    }
}
