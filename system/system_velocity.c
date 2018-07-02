#include "system_velocity.h"

void
add_velocity(PositionComponentPool *position_component_pool, VelocityComponentPool *velocity_component_pool) 
{
    int index = 0;
    while (index < 256) {
         position_component_pool->x_position[index] += velocity_component_pool->x_velocity[index];
         position_component_pool->y_position[index] += velocity_component_pool->y_velocity[index];
         
         index++;
    }
}