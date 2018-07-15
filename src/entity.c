#include <string.h>
#include <stdlib.h>

#include "des.h"

EntityPool
*entity_pool_create(int size)
{
    EntityPool *entity_pool = malloc(sizeof(EntityPool));

    if (entity_pool == NULL) {
    	return NULL;
    }

    entity_pool->size = size;
    entity_pool->guid = calloc(size, sizeof(*entity_pool->guid));
    entity_pool->component_pool = malloc(size*sizeof(*entity_pool->component_pool));
    entity_pool->component_index = malloc(size*sizeof(*entity_pool->component_index));

    if ( (entity_pool->guid == NULL) || (entity_pool->component_pool == NULL)  || (entity_pool->component_index == NULL) ) {
        entity_pool_destroy(entity_pool);
        return NULL;
    }

    return entity_pool;
}


void entity_pool_destroy(EntityPool *entity_pool)
{
    free(entity_pool->guid);
    free(entity_pool->component_pool);
    free(entity_pool->component_index);

    free(entity_pool);
}

int
entity_pool_find_empty_row(EntityPool *entity_pool)
{
    int index = 0;
    while (entity_pool->guid[index] && !(index >= entity_pool->size)) {
        index++;
    }

    if (index >= entity_pool->size) {
        return -1;
    }

    return index;
}

int
entity_create(EntityPool *entity_pool)
{
    int index = entity_pool_find_empty_row(entity_pool);
    if (index == -1) {
        return -1;
    }

    entity_pool->lastGUID++;
    entity_pool->guid[index] = entity_pool->lastGUID;

    return entity_pool->lastGUID;
}

int
component_add_to_entity_ID(EntityPool *entity_pool, int guid, MetaComponentPool *meta_component_pool)
{
    int index = entity_pool_find_empty_row(entity_pool);
    int component_index = component_add_to_entity_index(entity_pool, index, guid, meta_component_pool);

    return component_index;
}

int
component_add_to_entity_index(EntityPool *entity_pool, int index, int guid, MetaComponentPool* meta_component_pool)
{
    int component_index = component_pool_get_open_slot(meta_component_pool);
    if (component_index > -1) {
        component_pool_set_slot(meta_component_pool, component_index, 1);
    }

    entity_pool->guid[index] = guid;
    entity_pool->component_pool[index] = meta_component_pool;
    entity_pool->component_index[index] = component_index;

    return component_index;
}

int
component_remove_from_entity_index(EntityPool *entity_pool, int index)
{
    if (index > entity_pool->size) {
        return -1;
    }
    component_pool_set_slot(entity_pool->component_pool[index], entity_pool->component_index[index], 0);

    entity_pool->guid[index] = 0;
    entity_pool->component_pool[index] = NULL;
    entity_pool->component_index[index] = 0;

    return 0;
}

int
component_remove_from_entity_ID(EntityPool *entity_pool, int guid, MetaComponentPool *meta_component_pool)
{
    int index = 0;
    while (!(guid == entity_pool->guid[index]) |
           !(entity_pool->component_pool[index] == meta_component_pool)) {
        index++;
    }

    int code = component_remove_from_entity_index(entity_pool, index);
    return code;
}

void
entity_remove(EntityPool *entity_pool, int guid)
{
    int index = 0;
    while (index <= entity_pool->size) {
        if (entity_pool->guid[index] == guid) {
            component_remove_from_entity_index(entity_pool, index);
        }

        index++;
    }
}

