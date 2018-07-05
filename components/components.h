#ifndef COMPONENTS_H
#define COMPONENTS_H

typedef struct
PositionComponentPool
{
    float *x_position;
    float *y_position;
} PositionComponentPool;

typedef struct
VelocityComponentPool
{
    float *x_velocity;
    float *y_velocity;
} VelocityComponentPool;

#endif // COMPONENTS_H
