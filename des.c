#include <string.h>
#include <stdlib.h>

#include "des.h"

int lastGUID = 0;

typedef struct
EntityPool
{
    int guid[ENTITY_POOL_SIZE];
    MetaComponentPool *component_pool[ENTITY_POOL_SIZE];
    int component_data_id[ENTITY_POOL_SIZE];
} EntityPool;

EntityPool ENTITY_POOL;

int
entity_pool_find_empty_row()
{
    int index = 0;
    while (ENTITY_POOL.guid[index] && !(index >= ENTITY_POOL_SIZE)) {
        index++;
    }

    if (index >= ENTITY_POOL_SIZE) {
        return -1;
    }

    return index;
}

int
entity_create()
{
    int index = entity_pool_find_empty_row();
    if (index == -1) {
        return -1;
    }

    lastGUID++;
    ENTITY_POOL.guid[index] = lastGUID;

    return lastGUID;
}

int
component_add_to_entity_ID(int guid, MetaComponentPool *meta_component_pool)
{
    int index = entity_pool_find_empty_row();
    int component_index = component_add_to_entity_index(index, guid, meta_component_pool);

    return component_index;
}

int
component_add_to_entity_index(int index, int guid, MetaComponentPool* meta_component_pool)
{
    ENTITY_POOL.guid[index] = guid;
    ENTITY_POOL.component_pool[index] = meta_component_pool;

    int component_index = component_pool_get_open_slot(meta_component_pool);
    if (component_index > -1) {
        component_pool_set_slot(meta_component_pool, component_index, 1);
    }

    return component_index;
}

void
component_remove_from_entity_index(int index)
{
    // TODO: Error code: index out of bounds
    component_pool_set_slot(ENTITY_POOL.component_pool[index], ENTITY_POOL.component_data_id[index], 0);

    ENTITY_POOL.guid[index] = 0;
    ENTITY_POOL.component_pool[index] = NULL;
    ENTITY_POOL.component_data_id[index] = 0;
}

void
component_remove_from_entity_ID(int guid, MetaComponentPool *meta_component_pool)
{
    int index = 0;
    while (!(guid == ENTITY_POOL.guid[index]) |
           !(ENTITY_POOL.component_pool[index] == meta_component_pool)) {
        index++;
    }

    component_remove_from_entity_index(index);
}

void
entity_remove(int guid)
{
    int index = 0;
    while (index <= ENTITY_POOL_SIZE) {
        if (ENTITY_POOL.guid[index] == guid) {
            component_remove_from_entity_index(index);
        }

        index++;
    }
}

void
entity_get_components(int guid, MetaComponentPool *component_pool[ENTITY_COMPONENT_LIMIT]) 
{
    // TODO: remove limit, user will use sizeof
    // TODO: fix component pool vs components
    int index = 0;
    int component_index = 0;
    while (index <= ENTITY_POOL_SIZE) {
        if (ENTITY_POOL.guid[index] == guid) {
            component_pool[component_index] = ENTITY_POOL.component_pool[index];
            component_index++;
       }

        index++;
    }
}

MetaComponentPool
component_pool_register(void *component_pool, int size)
{
    MetaComponentPool meta_component_pool;
    meta_component_pool.size = size;
    meta_component_pool.mask = malloc(size);
    meta_component_pool.component_pool = component_pool;

    return meta_component_pool;
}

int
component_pool_get_open_slot(MetaComponentPool *meta_component_pool)
{
    for (int i = 0; i < meta_component_pool->size / 64; i++) {
        u64 *block = meta_component_pool->mask + i;

        int free_slot = __builtin_ffsll(~*block);

        if (free_slot > 0) {
            // Free slot returns a value of 1 if the first bit is "on"
            return i * 64 + free_slot - 1;
        }
    }

    return -1;
}

int
component_pool_get_slot(MetaComponentPool *meta_component_pool, int index)
{
    if (index > meta_component_pool->size) {
        return -1;
    }

    int block_index = index / 64;
    int offset = index % 64;
    u64 *block = meta_component_pool->mask + block_index;
    int result = *block & 1ULL << offset;

    if (result) {
        return 1;
    }

    return 0;
}

int
component_pool_set_slot(MetaComponentPool *meta_component_pool, int index, int state) 
{
    if (index > meta_component_pool->size) {
        return -1;
    }

    int block_index = index / 64;
    int offset = index % 64;

    u64 *block = meta_component_pool->mask + block_index;

    if (state) {
        *block |= 1ULL << offset;
    } else {
        *block &= ~(1ULL << offset);
    }

    return 0;
}
