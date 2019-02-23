#include "wrapper.h"

#define QUEUE_NAME "/mq1"

int main()
{
	//Implement you inter process communication here, happy coding
    planet_type planet;
    mqd_t mq;

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

        if(MQconnect(&mq, QUEUE_NAME) != 1){
            printf("CLIENT: Could not connect!\n");
        }
        else{
            printf("CLIENT: Connected!\n");
            MQwrite(&mq, (void *)&planet);
            printf("CLIENT: Request sent!\n");
        }
    }
}
