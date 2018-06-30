#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "des.h"

typedef struct positionComponent {
    float x[200];
    float y[200];
} positionComponent;

typedef struct velocityComponent {
    float x[200];
    float y[200];
} velocityComponent;

positionComponent posComp;
velocityComponent velComp;

void velocitySystem() {
    int idx = 0;
    while (idx < 200) {
         posComp.x[idx] += velComp.x[idx];
         posComp.y[idx] += velComp.y[idx];
         idx++;
    }
}

int main () {
    srand(time(NULL));

    for (int i=0;i<50;i++) {
        int guid = createEntity();
        add_component_to_entity_ID(guid, &posComp);
        add_component_to_entity_ID(guid, &velComp);

        posComp.x[i] = rand();
        posComp.y[i] = rand();
        velComp.x[i] = rand();
        velComp.y[i] = rand();
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

    return 0;
}
