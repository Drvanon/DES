#include "des.h"
#include "assemblage.h"

#ifndef ASSEMBLAGE_POOL_SIZE
#define ASSEMBLAGE_POOL_SIZE 100
#endif

typedef struct assemblage_pool {
    int assemblage_id[ASSEMBLAGE_POOL_SIZE];
    void* component[ASSEMBLAGE_POOL_SIZE];
} assemblage_pool;

assemblage_pool ASSEMBLAGE_POOL;

int find_empty_asssemblage_row () {
    int idx = 0;
    while ((idx < ASSEMBLAGE_POOL_SIZE) && !ASSEMBLAGE_POOL[idx]) {
        idx++;
    }
    if (idx >= ASSEMBLAGE_POOL_SIZE) {
        return -1;
    }
    return idx;
}

int register_assemblage_component (int assemblage_id, void* component) {
    idx = find_empty_asssemblage_row();
    if (idx < 0) {
        return -1;
    }

    ASSEMBLAGE_POOL[idx].assemblage_id = assemblage_id;
    ASSEMBLAGE_POOL[idx].compontents[idx] = component;

    return idx;
}

int remove_assemblage_component(int assemblage_id, void* component) {
    idx = find_empty_asssemblage_row();
    if (idx < 0) {
        return -1;
    }

    ASSEMBLAGE_POOL.assemblage_id[idx] = 0;
}

int create_entity_from_assemblage (int assemblage_id) {
    int guid = createEntity();
    for (int idx=0;idx<ASSEMBLAGE_POOL_SIZE;idx++) {
        if (assemblage_id == ASSEMBLAGE_POOL[idx]) {
            add_component_to_entity_ID(guid, ASSEMBLAGE_POOL.component[idx]);
        }
    }
}
