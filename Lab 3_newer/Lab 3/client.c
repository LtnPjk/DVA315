#include "wrapper.h"

int main()
{
	//Implement you inter process communication here, happy coding
    planet_type planet;

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
    }
}
