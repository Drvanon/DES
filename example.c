#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "int.h"
#include "des.h"

typedef struct
positionComponent {
    u64* space;
    int size = 200;
    float x[size];
    float y[size];
} positionComponent;

typedef struct
velocityComponent {
    u64* space;
    int size = 200;
    float x[size];
    float y[size];
} velocityComponent;

void
velocitySystem() {
    int idx = 0;
    while (idx < 200) {
         posComp.x[idx] += velComp.x[idx];
         posComp.y[idx] += velComp.y[idx];
         idx++;
    }
}

    float x[200];
    float y[200];
} velocityComponent;

void
velocitySystem() {
    int idx = 0;
    while (idx < 200) {
         posComp.x[idx] += velComp.x[idx];
         posComp.y[idx] += velComp.y[idx];
         idx++;
    }
}

int
main () {
    srand(time(NULL));

    positionComponent posComp;
    long long posMask[];

    velocityComponent velComp;
    long long posMask[];

    for (int i=0;i<50;i++) {
        int guid = createEntity();
        int posIdx = add_component_to_entity_ID(guid, &posComp, mask);
        int velIdx = add_component_to_entity_ID(guid, &velComp, mask);

        posComp.x[posIdx] = rand();
        posComp.y[posIdx] = rand();
        velComp.x[velIdx] = rand();
        velComp.y[velIdx] = rand();
    }

    int i = 0;
    while (i<2000) {
        velocitySystem();
        i++;
    }

    i = 0;
    while (i<200) {
        printf("%.1f, %.1f \n", posComp.x[i], posComp.y[i]);
        printf("%.1f, %.1f \n", velComp.x[i], velComp.y[i]);
        printf("-----\n");
        i++;
    }

    free(posMask);
    free(velMask)

    return 0;
}
