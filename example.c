#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "int.h"
#include "des.h"

typedef struct
positionComponent {
    float *x;
    float *y;
} positionComponent;

typedef struct
velocityComponent {
    float *x;
    float *y;
} velocityComponent;

void
velocitySystem(positionComponent *posComp, velocityComponent *velComp) {
    int idx = 0;
    while (idx < 256) {
         posComp->x[idx] += velComp->x[idx];
         posComp->y[idx] += velComp->y[idx];
         idx++;
    }
}

int
main () {
    positionComponent posComp;
    velocityComponent velComp;
    srand(time(NULL));

    metaComponent posMetComp = register_component(&posComp, 256);
    metaComponent velMetComp = register_component(&velComp, 256);
    posComp.x = malloc(256*sizeof(float));
    posComp.y = malloc(256*sizeof(float));
    velComp.x = malloc(256*sizeof(float));
    velComp.y = malloc(256*sizeof(float));

    for (int i=0;i<50;i++) {
        int guid = createEntity();
        int posIdx = add_component_to_entity_ID(guid, &posMetComp);
        int velIdx = add_component_to_entity_ID(guid, &velMetComp);
        if (posIdx == -1 | velIdx == -1){
            printf("Got some internal error");
        }

        posComp.x[posIdx] = (rand() % 100) / (float)100;
        posComp.y[posIdx] = (rand() % 100) / (float)100;
        velComp.x[velIdx] = (rand() % 100) / (float)100;
        velComp.y[velIdx] = (rand() % 100) / (float)100;
    }


    int i = 0;
    while (i<2000) {
        velocitySystem(&posComp, &velComp);
        i++;
    }

    i = 0;
    printf("ID \t|Posx \t|Posy \t|Velx \t|Vely \t|mask\n");
    printf("-------------------------------------------------------\n");
    while (i<256) {
        printf("%d \t| %.1f \t| %.1f \t| %.1f \t|%.1f \t|%d\n", i, posComp.x[i], posComp.y[i], velComp.x[i], velComp.y[i], 1);
        i++;
    }
    unsigned long long test = 1;
    printf("%lu", sizeof(test));

    return 0;
}
