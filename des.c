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

/* @brief Retrieve a new guid for an entity and register the change
 *
 * @return int guid
 */
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

/* @brief Create a new row in the component-entity pool
 *
 * This function takes a previously established GUID and a MetaComponentPool
 * to create a new row in the component-entity pool. This is to be used
 * in combination with createEntity, after having regisetered your
 * component.
 *
 * @code
 * int your_component_pool_size = 200; // Amount of rows the componentPool can have
 * MetaComponentPoolPool YourMetaComponentPoolPool = component_register(*YourComponentPool, your_component_pool_size);
 * int guid = entity_create();
 * int component_index = component_add_to_entity_ID(guid, &YourMetaComponentPoolPool);
 * YourComponentPool.member1[component_index] = "Some content";
 * @endcode
 *
 * @param int guid
 * @param metaComponent*
 *
 * @return int This variable refers to the index of the user defined component pool that can be set to the prefered data, see example above.
 */
int
add_component_to_entity_ID (int guid, metaComponent *metComp) {
    int idx = find_empty_row();

    int data_idx = add_component_to_entity_IDX(idx, guid, metComp);

    return data_idx;
}

/* @brief Add a component to an entity if the user has already obtained an empty row.
 *
 * It is possible that the user will be aware of the row
 * that the entity was created in, so this is a copy of
 * add_component_to_entity_ID, but works based of index instead
 * of id, which is much faster, for we will not have to go
 * trough the entire entity pool.
 *
 * @param int index of the entity-component pool
 * @param int guid of the new entity
 * @param MetaComponentPool* MetaComponentPool of the component that is to be added to the entity
 *
 * @return int Index of component pool that can be used to store the data the user wants to store
 */
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

/* @brief Remove the row of the given index in the component-entity pool.
 * @param int index
 * @return int Error codes: 0 is okay, -1 is index out of bounds
 */
int
remove_entity_component_index (int idx) {
    // TODO: Error code: index out of bounds
    set_slot_in_component(ENTITY_POOL.component[idx], ENTITY_POOL.component_data_id[idx], 0);

    ENTITY_POOL.guid[idx] = 0;
    ENTITY_POOL.component[idx] = NULL;
    ENTITY_POOL.component_data_id[idx] = 0;
    return 0;
}

/* @brief Remove a single component from the specified entity
 *
 * @param guid
 * @param MetaComponentPool
 * @return int Error codes: 0 is okay, -1 means could not find entity-component in pool.
 */
int
remove_component_from_entity (int guid, metaComponent *metComp) {
    int idx = 0;
    while (!(guid == ENTITY_POOL.guid[idx]) | !(ENTITY_POOL.component[idx] == metComp)) {
        idx++;
    }

    remove_entity_component_index(idx);
    return 0;
}

/* @brief Remove all data on the entity.
 *
 * @param guid
 * @return void
 */
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

/* @brief Retrieve all components linked to the entity in the provided components list.
 *
 * @param guid
 * @param MetaComponentPool
 * @param *components[]
 *
 * @return void
 */
void
get_entity_components (int guid, metaComponent *components[ENTITY_COMPONENT_LIMIT]) {
    // TODO: remove limit, user will use sizeof
    // TODO: fix component pool vs components
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

/* @brief Register a component for use in the DES system.
 *
 * The MetaComponentPool is used to provide an abstraction layer between the DES and the user.
 * It provides information on what index of the user defined component pool is open and
 * what row is already in use by an entity. It also gives information on the size of the
 * pool and refers to the component pool.
 *
 * @param void* Pointer to the user-defined component_pool.
 * @param int Size of the memory allocated for the pools.
 *
 * @return MetaComponentPool
 */
metaComponent
register_component (void *component, int size) {
    metaComponent metComp;

    metComp.size = size;
    metComp.mask = malloc(size);
    metComp.component = component;

    return metComp;
}

/* @brief Find the first slot in the mask that has not been filled.
 *
 * @param MetaComponentPool*
 *
 * @return int component index of the open slot.
 */
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

/* @brief Find the state of a slot in a component pool
 *
 * This function will tell the user if the slot is in vacant or not.
 *
 * @param MetaComponentPool*
 * @param index
 *
 * @return int bool value
 */
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

/* @brief Set slot state in meta component to be vacant or free
 *
 * @param MetaComponentPool* Meta of the component pool
 * @param int Index of the slot
 * @param int Desired state of the slot
 *
 * @return Error codes: 0 is okay, -1 is index out of bounds
 */
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

