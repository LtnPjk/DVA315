#include "wrapper.h"
#include <unistd.h>

int main(){
    for(int i = 0; i < 10; i++){
        printf("Hello world!\n");
        sleep(1);
    }
}
