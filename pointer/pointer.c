#include <stdio.h>
#include <stdlib.h>

int main(){
    int *c1 = (char*)malloc(1);
    int *c2 = c1+1;
    unsigned int res = c2 - c1;
    printf("%u \n", c1);
    printf("%u \n", c1);
    printf("%u \n", res);

    return 0 ;
}