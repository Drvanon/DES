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
    test_component_pool.member1 = malloc(component_pool_size*sizeof(int));
    test_component_pool.member2 = malloc(component_pool_size*sizeof(int));

    // Test for succesfull registration
    MetaComponentPool meta_test_pool = component_pool_register(&test_component_pool, component_pool_size);
    assert(meta_test_pool.component_pool == &test_component_pool);

    // Make sure that a valid GUID is returned
    int guid = entity_create(entity_pool);
    assert(guid > 0);

    // Check that the EntityPool is empty when initialized
    EntityPool *components_of_guid = entity_get_components(entity_pool, guid, 100);
    for (int i=0;i<100;i++) {
        assert(components_of_guid->guid == 0);
    }

    // Assert that the entity pool is fully destroyed
    entity_pool_destroy(components_of_guid);
    assert(components_of_guid == NULL);

    // Check for a valid component and mask integrity
    int component_index = component_add_to_entity_ID(entity_pool, guid, &meta_test_pool);
    assert(( component_index >= 0 ) && ( component_index < meta_test_pool.size ));
    // The set index is in use
    assert(component_pool_get_slot(meta_test_pool, component_index) == 1);
    // All other indexes are vacant
    for (int i = 0;i<component_pool_size;i++) {
        if (!(i == component_index)) {
            assert(component_pool_get_slot(meta_test_pool, i) == 0);
        }
    }

    entity_pool_destroy(components_of_guid);
    assert(components_of_guid == NULL);

    entity_pool_destroy(entity_pool);

    printf("All tests ran successfully\n");

    return 0;
}
