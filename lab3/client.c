#include "wrapper.h"

int main()
{
	//Implement you inter process communication here, happy coding
    planet_type planet;
    mqd_t mq;
    sem_t *empty = sem_open(SEM_EMPTY, O_RDWR);
    sem_t *full = sem_open(SEM_FULL, O_RDWR);

    if(MQconnect(&mq, QUEUE_NAME) != 1){
        printf("CLIENT: Could not connect!\n");
    }
    else{
        printf("CLIENT: Connected!\n");

    while(1){
        printf("---  Create Planet  ---\n");
        printf("Name:\tx-pos\ty-pos\tx-vel\ty-vel\tlife\n");
        scanf("%s",  planet.name);
        scanf("%lf", &planet.sx);
        scanf("%lf", &planet.sy);
        scanf("%lf", &planet.vx);
        scanf("%lf", &planet.vy);
        scanf("%d", &planet.life);
        printf("Spawning planet...\n");
        sem_wait(empty);
        MQwrite(&mq, (void *)&planet);
        printf("CLIENT: Request sent!\n\n");
        sem_post(full);
        }
    }
}
