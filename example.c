#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "des.h"
#include "components.h"
#include "system_velocity.h"

int
main() 
{
    PositionComponentPool position_component_pool;
    VelocityComponentPool velocity_component_pool;
    srand(time(NULL));

    MetaComponentPool position_meta_component = component_pool_register(&position_component_pool, 256);
    MetaComponentPool velocity_meta_component = component_pool_register(&velocity_component_pool, 256);
    position_component_pool.x_position = malloc(256*sizeof(float));
    position_component_pool.y_position = malloc(256*sizeof(float));
    velocity_component_pool.x_velocity = malloc(256*sizeof(float));
    velocity_component_pool.y_velocity = malloc(256*sizeof(float));

    for (int i = 0; i < 50; i++) {
        int guid = createEntity();
        int position_index = add_component_to_entity_ID(guid, &position_meta_component);
        int velocity_index = add_component_to_entity_ID(guid, &velocity_meta_component);
        if (position_index == -1 | velocity_index == -1){
            printf("Got some internal error");
        }

        position_component_pool.x_position[position_index] = (rand() % 100) / 100.0;
        position_component_pool.y_position[position_index] = (rand() % 100) / 100.0;
        velocity_component_pool.x_velocity[velocity_index] = (rand() % 100) / 100.0;
        velocity_component_pool.y_velocity[velocity_index] = (rand() % 100) / 100.0;
    }


    for (int i = 0; i < 2000; i++) {
        add_velocity(&position_component_pool, &velocity_component_pool);
    }

    printf("ID \t|Posx \t|Posy \t|Velx \t|Vely \t|mask\n");
    printf("-------------------------------------------------------\n");
    for (int i = 0; i < 256; i++) {
        printf("%d \t| %.1f \t| %.1f \t| %.1f \t|%.1f \t|%d\n", 
                i, 
                position_component_pool.x_position[i], 
                position_component_pool.y_position[i], 
                velocity_component_pool.x_velocity[i],
                velocity_component_pool.y_velocity[i], 
                1);
    }

    return 0;
}
