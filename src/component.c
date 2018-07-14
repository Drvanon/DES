#include "des.h"
#include <stdlib.h>

EntityPool
*entity_get_components(EntityPool *entity_pool, int guid, int max_query_size)
{
    EntityPool *ret_entity_pool = entity_pool_create(max_query_size);
    int index = 0;
    int ret_index = 0;
    while ( (index <= entity_pool->size) & (index < max_query_size) ) {
        if (entity_pool->guid[index] == guid) {
            // Copy the relevant row to the returning entity_pool
            ret_entity_pool->guid[ret_index] = entity_pool->guid[index];
            ret_entity_pool->component_pool[ret_index] = entity_pool->component_pool[index];
            ret_entity_pool->component_index[ret_index] = entity_pool->component_index[index];
            ret_index++;
       }

        index++;
    }

    return ret_entity_pool;
}

MetaComponentPool
component_pool_register(void *component_pool, int size)
{
    MetaComponentPool meta_component_pool;
    meta_component_pool.size = size;
    meta_component_pool.mask = calloc(size, size);
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
