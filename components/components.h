#ifndef COMPONENTS_H
#define COMPONENTS_H

/**
 * @brief Component for the position of the entity
 * 
 */
typedef struct
PositionComponentPool
{
    float *x_position;
    float *y_position;
} PositionComponentPool;

/**
 * @brief Component with the velocity data. This gets added to the position by a system
 * 
 */
typedef struct
VelocityComponentPool
{
    float *x_velocity;
    float *y_velocity;
} VelocityComponentPool;

#endif // COMPONENTS_H
