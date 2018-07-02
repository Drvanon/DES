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
assemblage_components_register(int assemblage_id, void* component) {
    int idx = find_empty_asssemblage_row();
    if (idx < 0) {
        return -1;
    }

    ASSEMBLAGE_POOL.assemblage_id[idx] = assemblage_id;
    ASSEMBLAGE_POOL.meta_component_pool[idx] = component;

    return idx;
}

int
remove_assemblage_component(int assemblage_id, void* component) {
    int idx = find_empty_asssemblage_row();
    if (idx < 0) {
        return -1;
    }

    ASSEMBLAGE_POOL.assemblage_id[idx] = 0;
    return 0;
}

int
create_entity_from_assemblage (int assemblage_id) {
    int guid = entity_create();
    for (int idx=0;idx<ASSEMBLAGE_POOL_SIZE;idx++) {
        if (assemblage_id == ASSEMBLAGE_POOL.assemblage_id[idx]) {
            add_component_to_entity_ID(guid, ASSEMBLAGE_POOL.component[idx]);
        }
    }
    return 0;
}
