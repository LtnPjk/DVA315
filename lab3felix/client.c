#include "COMMON/wrapper.h"

int main()
{
    //Implement you inter process communication here, happy coding
    planet_type planet;
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

        if(MQconnect(&mq, QUEUE_NAME) != 1){
            printf("CLIENT: Could not connect!\n");
        }
        else{
            sem_wait(sem_empty);
            sem_wait(sem_mutex);
            printf("CLIENT: Connected!\n");
            p = MQwrite(&mq, (void *)&planet);
            printf("CLIENT: Request returned: %d\n", p);
            if(p==0){
                printf("ERROR: %d\n", errno);
            }
            sem_post(sem_mutex);
            sem_post(sem_full);
        }
    }
}

/*#include "COMMON/wrapper.h"

//#define QUEUE_NAME "/mq1"

int main()
{
    //Implement you inter process communication here, happy coding
    planet_type planet;
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

        if(MQconnect(&mq, QUEUE_NAME) != 1){
            printf("CLIENT: Could not connect!\n");
        }
        else{
            sem_wait(sem_empty);
            sem_wait(sem_mutex);
            printf("CLIENT: Connected!\n");
            p = MQwrite(&mq, (void *)&planet);
            printf("CLIENT: Request sent!\n");
            sem_post(sem_mutex);
            sem_post(sem_full);
        }
    }
}*/