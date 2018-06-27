#ifndef ENTITY_POOL_SIZE
#define ENTITY_POOL_SIZE 100
#endif
#ifndef ENTITY_COMPONENT_LIMIT
#define ENTITY_COMPONENT_LIMIT 50
#endif

typedef struct entity_pools {
    int entity_id[ENTITY_POOL_SIZE];
    void* component[ENTITY_POOL_SIZE];
    int component_data_id[ENTITY_POOL_SIZE];
} entity_pool;

entity_pool ENTITY_POOL;

int add_component_to_entity(int guid, void* component, int component_data_id) {
    int idx = 0;
    while (ENTITY_POOL.entity_id[idx] && !idx >= ENTITY_POOL_SIZE) {
        idx++;
    }
    if (idx >= ENTITY_POOL_SIZE) {
        return -1;
    }
    ENTITY_POOL.entity_id[idx] = guid;
    ENTITY_POOL.component[idx] = component;
    ENTITY_POOL.component_data_id[idx] = component_data_id;
    return idx;
}

int remove_component_from_entity(int idx) {
    // Sanity check
    if (idx >= ENTITY_POOL_SIZE) {
        return -1;
    }
    ENTITY_POOL.entity_id[idx] = 0;
    return 0;
}

void remove_entity(int guid) {
    int idx = 0;
    while (idx <= ENTITY_POOL_SIZE) {
        if (ENTITY_POOL.entity_id[idx] == guid) {
            ENTITY_POOL.entity_id[idx] = 0;
        }
        idx++;
    }
}

void get_entity_components (int guid, void* components[ENTITY_COMPONENT_LIMIT]) {
    int idx = 0;
    int component_idx = 0;
    while (idx <= ENTITY_POOL_SIZE) {
        if (ENTITY_POOL.entity_id[idx] == guid) {
            components[component_idx] = ENTITY_POOL.components[idx];
            component_idx++;
        }
        idx++;
    }
}
