#include <string.h>
#include "des.h"
#include "int.h"

#ifndef ENTITY_POOL_SIZE
#define ENTITY_POOL_SIZE 100
#endif
#ifndef ENTITY_COMPONENT_LIMIT
#define ENTITY_COMPONENT_LIMIT 50
#endif

int lastGUID = 0;
int lastComponentID = 0;

typedef struct
entity_pools {
    int guid[ENTITY_POOL_SIZE];
    void* component[ENTITY_POOL_SIZE];
    int component_data_id[ENTITY_POOL_SIZE];
} entity_pool;

entity_pool ENTITY_POOL;

int
find_empty_row() {
    int idx = 0;
    while (ENTITY_POOL.guid[idx] && !(idx >= ENTITY_POOL_SIZE)) {
        idx++;
    }

    if (idx >= ENTITY_POOL_SIZE) {
        return -1;
    }

    return idx;
}

int
createEntity() {
    int idx = find_empty_row();
    if (idx == -1) {
        return -1;
    }

    lastGUID++;
    ENTITY_POOL.guid[idx] = lastGUID;

    return lastGUID;
}

int
add_component_to_entity_ID (int guid, void* component) {
    int idx = find_empty_row();

    int data_idx = add_component_to_entity_IDX(idx, guid, component);

    return data_idx;
}

/* It is very possible that the user will be aware of the row
   that the entity was created in, so this is a copy of
   add_component_to_entity_ID, but works based of index instead
   of id, which is much faster, for we will not have to go
   trough the entire entity pool. */
int
add_component_to_entity_IDX (int idx, int guid, void* component) {
    ENTITY_POOL.guid[idx] = guid;
    ENTITY_POOL.component[idx] = component;

    // ffsll(component->mask);

    return 0;
}

int
remove_component_from_entity (int idx) {
    // Sanity check
    if (idx >= ENTITY_POOL_SIZE) {
        return -1;
    }

    ENTITY_POOL.guid[idx] = 0;
    return 0;
}

void
remove_entity (int guid) {
    int idx = 0;
    while (idx <= ENTITY_POOL_SIZE) {
        if (ENTITY_POOL.guid[idx] == guid) {
            ENTITY_POOL.guid[idx] = 0;
        }
        idx++;
    }
}

void
get_entity_components (int guid, void* components[ENTITY_COMPONENT_LIMIT]) {
    int idx = 0;
    int component_idx = 0;
    while (idx <= ENTITY_POOL_SIZE) {
        if (ENTITY_POOL.guid[idx] == guid) {
            components[component_idx] = ENTITY_POOL.component[idx];
            component_idx++;
        }
        idx++;
    }
}
