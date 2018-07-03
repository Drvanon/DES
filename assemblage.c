#include "des.h"
#include "assemblage.h"

#ifndef ASSEMBLAGE_POOL_SIZE
#define ASSEMBLAGE_POOL_SIZE 100
#endif

typedef struct
AssemblagePool {
    int assemblage_id[ASSEMBLAGE_POOL_SIZE];
    MetaComponentPool meta_component_pool[ASSEMBLAGE_POOL_SIZE];
} AssemblagePool;

AssemblagePool ASSEMBLAGE_POOL;

int
assemblage_row_find_empty()
{
    int index = 0;
    while ((index < ASSEMBLAGE_POOL_SIZE) && !ASSEMBLAGE_POOL.assemblage_id[index]) {
        index++;
    }
    if (index >= ASSEMBLAGE_POOL_SIZE) {
        return -1;
    }

    return index;
}

int
assemblage_register_component(int assemblage_id, void* component)
{
    int index = assemblage_row_find_empty();
    if (index < 0) {
        return -1;
    }

    ASSEMBLAGE_POOL.assemblage_id[index] = assemblage_id;
    ASSEMBLAGE_POOL.meta_component_pool[index] = component;

    return index;
}

int
assemblage_remove_component(int assemblage_id, void* component)
{
    int index = find_empty_asssemblage_row();
    if (index < 0) {
        return -1;
    }

    ASSEMBLAGE_POOL.assemblage_id[index] = 0;
    return 0;
}

int
assemblage_create_entity(int assemblage_id)
{
    int guid = entity_create();
    for (int i = 0; i < ASSEMBLAGE_POOL_SIZE; i++) {
        if (assemblage_id == ASSEMBLAGE_POOL.assemblage_id[i]) {
            component_add_to_entity_ID(guid, ASSEMBLAGE_POOL.meta_component_pool[i]);
        }
    }

    return guid;
}
