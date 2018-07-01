#include <string.h>
#include <stdlib.h>
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
    metaComponent *component[ENTITY_POOL_SIZE];
    int component_data_id[ENTITY_POOL_SIZE];
} entity_pool;

entity_pool ENTITY_POOL;

int
find_empty_row () {
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
createEntity () {
    int idx = find_empty_row();
    if (idx == -1) {
        return -1;
    }

    lastGUID++;
    ENTITY_POOL.guid[idx] = lastGUID;

    return lastGUID;
}

int
add_component_to_entity_ID (int guid, metaComponent *metComp) {
    int idx = find_empty_row();

    int data_idx = add_component_to_entity_IDX(idx, guid, metComp);

    return data_idx;
}

/* It is very possible that the user will be aware of the row
   that the entity was created in, so this is a copy of
   add_component_to_entity_ID, but works based of index instead
   of id, which is much faster, for we will not have to go
   trough the entire entity pool. */
int
add_component_to_entity_IDX (int idx, int guid, metaComponent* metComp) {
    ENTITY_POOL.guid[idx] = guid;
    ENTITY_POOL.component[idx] = metComp;

    int data_idx = get_open_slot_in_component(metComp);
    if (data_idx > -1) {
        set_slot_in_component(metComp, data_idx, 1);
    }

    return data_idx;
}

int
remove_entity_component_index (int idx) {
    set_slot_in_component(ENTITY_POOL.component[idx], ENTITY_POOL.component_data_id[idx], 0);

    ENTITY_POOL.guid[idx] = 0;
    ENTITY_POOL.component[idx] = NULL;
    ENTITY_POOL.component_data_id[idx] = 0;
    return 0;
}

int
remove_component_from_entity (int guid, metaComponent *metComp) {
    int idx = 0;
    while (!(guid == ENTITY_POOL.guid[idx]) | !(ENTITY_POOL.component[idx] == metComp)) {
        idx++;
    }

    remove_entity_component_index(idx);
    return 0;
}

void
remove_entity (int guid) {
    int idx = 0;
    while (idx <= ENTITY_POOL_SIZE) {
        if (ENTITY_POOL.guid[idx] == guid) {
            remove_entity_component_index(idx);
        }
        idx++;
    }
}

void
get_entity_components (int guid, metaComponent *components[ENTITY_COMPONENT_LIMIT]) {
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

metaComponent
register_component (void *component, int size) {
    metaComponent metComp;

    metComp.size = size;
    metComp.mask = malloc(size);
    metComp.component = component;

    return metComp;
}

int
get_open_slot_in_component (metaComponent *metComp) {
    for (int i=0; i<metComp->size/64; i++) {
        u64 *block = metComp->mask + i;

        int free_slot = ffsll(~*block);

        if (free_slot > 0) {
            return i * 64 + free_slot - 1; // Free slot returns a value of 1 if the first bit is "on"
        }
    }
    return -1;
}

/* Find the state of a slot in a component */
int
get_slot_in_component (metaComponent *metComp, int index) {
    if (index > metComp->size) {
        return -1;
    }

    int block_index = index / 64;
    int offset = index % 64;
    u64 *block = metComp->mask + block_index;
    int result = *block & 1ULL << offset;

    if (result) {
        return 1;
    }
    return 0;
}

int
set_slot_in_component (metaComponent *metComp, int index, int state) {
    if (index > metComp->size) {
        return -1;
    }

    int block_index = index / 64;
    int offset = index % 64;

    u64 *block = metComp->mask + block_index;

    if (state) {
        *block |= 1ULL << offset;
    } else {
        *block &= ~(1ULL << offset);
    }

    return 0;
}

