#include <stdlib.h>
#include <time.h>

#define qModulo 17
#define smallMin 1
#define smallMax 15

int RandSmallCoeff(){
    return rand() % (smallMax - smallMin + 1) + smallMin;
}

int RandCoeff(){
    return rand() % qModulo;
}

void* CreatePublicKey(){
    srand(time(NULL));
    int x = 5;
    int f = x^4 + 1;

    int s[2] = {RandSmallCoeff()*-x^3-RandSmallCoeff()*x^2+x, -RandSmallCoeff()*x^2-x};
    
}