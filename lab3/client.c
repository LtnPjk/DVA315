#include "wrapper.h"

int main()
{
	//Implement you inter process communication here, happy coding
    mqd_t mq;

    if(MQconnect(&mq, QUEUE_NAME) != 1){
        printf("CLIENT: Could not connect!\n");
        return;
    }
    else{
        printf("CLIENT: Connected!\n");
    }
    while(1){
        printf("---  Create Planet  ---\n");
        printf("Name:\tx-pos\ty-pos\tx-vel\ty-vel\tlife\n");
        planet_type planet;
        scanf("%s",  planet.name);
        scanf("%lf", &planet.sx);
        scanf("%lf", &planet.sy);
        scanf("%lf", &planet.vx);
        scanf("%lf", &planet.vy);
        scanf("%lf", &planet.mass);
        scanf("%d", &planet.life);
        printf("Spawning planet...\n");

        MQwrite(&mq, (void *)&planet);
        if(strcmp(planet.name, "END") == 0){
            printf("CLIENT: END request recieved...\n");
            return MQclose(&mq, QUEUE_NAME);
        }
        printf("CLIENT: Request sent!\n");
    }
}
