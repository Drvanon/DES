#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "des.h"

typedef struct positionComponent {
    int x[200];
    int y[200];
} positionComponent;

typedef struct velocityComponent {
    int x[200];
    int y[200];
} velocityComponent;

positionComponent posComp;
velocityComponent velComp;

void velocitySystem() {

    int idx = 0;
    while (idx < 200) {
         posComp.x[idx] += velComp.x[idx];
         posComp.y[idx] += velComp.y[idx];
    }
}

int main () {
    srand(time(NULL));

    for (int i=0;i<50;i++) {
        posComp.x[i] = rand();
        posComp.y[i] = rand();
        velComp.x[i] = rand();
        velComp.y[i] = rand();
    }

    return 0;
}
