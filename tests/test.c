#include <stdio.h>
#include "../des.h"

typedef struct TestComponentPool {
    int *member1;
    int *member2;
} TestComponentPool;

/*
int main () {
    // Setup 1 component
     int size = 4*64;
    TestComponentPool test_component_pool;
    MetaComponentPool meta_test_pool = component_pool_register(&meta_test_pool, size);
    test_component_pool.member1 = malloc(size*sizeof(int));
    test_component_pool.member2 = malloc(size*sizeof(int));

    int guid = entity_create();
    int component_index = component_add_to_entity_ID(guid, &meta_test_pool);

    int expected_index = entity_get_components()

    return 0;
}*/
