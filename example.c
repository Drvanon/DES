#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "int.h"
#include "des.h"

typedef struct
positionComponent {
    u64* mask;
    int size;
    float *x;
    float *y;
} positionComponent;

typedef struct
velocityComponent {
    u64* mask;
    int size;
    float *x;
    float *y;
} velocityComponent;

void
velocitySystem(positionComponent *posComp, velocityComponent *velComp) {
    int idx = 0;
    while (idx < 200) {
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

    for (int i=0;i<50;i++) {
        int guid = createEntity();
        int posIdx = add_component_to_entity_ID(guid, &posComp);
        int velIdx = add_component_to_entity_ID(guid, &velComp);

        posComp.x[posIdx] = rand();
        posComp.y[posIdx] = rand();
        velComp.x[velIdx] = rand();
        velComp.y[velIdx] = rand();
    }

    int i = 0;
    while (i<2000) {
        velocitySystem(&posComp, &velComp);
        i++;
    }

    i = 0;
    while (i<200) {
        printf("%.1f, %.1f \n", posComp.x[i], posComp.y[i]);
        printf("%.1f, %.1f \n", velComp.x[i], velComp.y[i]);
        printf("-----\n");
        i++;
    }

    return 0;
}
