#include <stdio.h>
#include <stdlib.h>
#include "des.h"
#include <assert.h>

typedef struct TestComponentPool {
    int *member1;
    int *member2;
} TestComponentPool;

int main () {

    EntityPool *entity_pool = entity_pool_create(64);

    // Setup 1 component
    int component_pool_size = 4*64;
    TestComponentPool test_component_pool;
    MetaComponentPool meta_test_pool = component_pool_register(&meta_test_pool, component_pool_size);
    test_component_pool.member1 = malloc(component_pool_size*sizeof(int));
    test_component_pool.member2 = malloc(component_pool_size*sizeof(int));

    int guid = entity_create(entity_pool);
    assert(guid > 0);

    int component_index = component_add_to_entity_ID(entity_pool, guid, &meta_test_pool);
    assert(component_index >= 0);

    EntityPool *components_of_guid = entity_get_components(entity_pool, guid, 100);

    entity_pool_destroy(components_of_guid);
    assert(components_of_guid == NULL);

    entity_pool_destroy(entity_pool);

    return 0;
}
