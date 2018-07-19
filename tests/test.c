#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "des.h"

typedef struct TestComponentPool {
    int *member1;
    int *member2;
} TestComponentPool;

typedef struct Test2ComponentPool {
    int *member1;
    int *member2;
} Test2ComponentPool;

typedef struct Test3ComponentPool {
    int *member1;
    int *member2;
} Test3ComponentPool;

typedef struct MarkedComponentLinkedList MarkedComponentLinkedList;
struct MarkedComponentLinkedList {
    MarkedComponentLinkedList *next;
    MetaComponentPool *component;
    bool marked;
};


int main () {

    EntityPool *entity_pool = entity_pool_create(64);
    assert(entity_pool->size == 64);

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

    // Check that the entity is empty when initialized
    EntityPool *components_of_guid = entity_get_components(entity_pool, guid, 100);
    for (int i=0;i<100;i++) {
        printf("%d, %d\n", i, components_of_guid->guid[i]);
        assert(components_of_guid->guid[i] == 0);
    }

    // Assert that the entity pool is fully destroyed
    entity_pool_destroy(&components_of_guid);
    assert(components_of_guid == NULL);

    // Check for a valid component and mask integrity
    int component_index = component_add_to_entity_ID(entity_pool, guid, &meta_test_pool);
    assert(( component_index >= 0 ) && ( component_index < meta_test_pool.size ));
    // The set index is in use
    assert(component_pool_get_slot(&meta_test_pool, component_index) == 1);
    // All other indexes are vacant
    for (int i = 0;i<component_pool_size;i++) {
        if (!(i == component_index)) {
            assert(component_pool_get_slot(&meta_test_pool, i) == 0);
        }
    }

    Test2ComponentPool test2_component_pool;
    test2_component_pool.member1 = malloc(component_pool_size*sizeof(int));
    test2_component_pool.member2 = malloc(component_pool_size*sizeof(int));

    Test3ComponentPool test3_component_pool;
    test3_component_pool.member1 = malloc(component_pool_size*sizeof(int));
    test3_component_pool.member2 = malloc(component_pool_size*sizeof(int));

    MetaComponentPool meta_test2_pool = component_pool_register(&test2_component_pool, component_pool_size);
    MetaComponentPool meta_test3_pool = component_pool_register(&test3_component_pool, component_pool_size);

    int first_assemblage = assemblage_create();
    int second_assemblage = assemblage_create();
    int third_assemblage = assemblage_create();

    assemblage_register_component(first_assemblage, &meta_test_pool);
    assemblage_register_component(first_assemblage, &meta_test2_pool);

    assemblage_register_component(second_assemblage, &meta_test_pool);
    assemblage_register_component(second_assemblage, &meta_test2_pool);
    assemblage_register_component(second_assemblage, &meta_test3_pool);

    assemblage_register_component(third_assemblage, &meta_test2_pool);
    assemblage_register_component(third_assemblage, &meta_test3_pool);

    int first_guid = assemblage_create_entity(entity_pool, first_assemblage);
    EntityPool *search_results = entity_get_components(entity_pool, first_guid, 15);

    // Making a list for cyclability and code reusability
    MarkedComponentLinkedList second_component_item;
    second_component_item.component = &meta_test2_pool;
    second_component_item.marked = false;
    second_component_item.next = NULL;

    MarkedComponentLinkedList first_component_item;
    first_component_item.component = &meta_test_pool;
    first_component_item.marked = false;
    first_component_item.next = &second_component_item;

    printf("\nguid \t| compent_pool \n");
    for (int i =0;i<entity_pool->size;i++) {
        printf("%d \t| %p\n", entity_pool->guid[i], (void *)entity_pool->component_pool[i]);
    }


    MarkedComponentLinkedList *head;
    for (int i=0;i<search_results->size;i++) {
        head = &first_component_item;
        while (head) {
            if (head->component == search_results->component_pool[i]) {
                head->marked = true;
                break;
            }

            head = head->next;
        }
    }
    entity_pool_destroy(&search_results);

    head = &first_component_item;
    while(head) {
        assert(head->marked == true);
        head = head->next;
    }

    int second_guid = assemblage_create_entity(entity_pool, second_assemblage);
    int third_guid = assemblage_create_entity(entity_pool, third_assemblage);

    printf("\nguid \t| compent_pool \n");
    for (int i =0;i<entity_pool->size;i++) {
        printf("%d \t| %p\n", entity_pool->guid[i], (void *)entity_pool->component_pool[i]);
    }

    MetaComponentPool *components[] = {&meta_test_pool, &meta_test2_pool};
    int *guids =  components_get_all_entities(entity_pool, components, 2);

    // Clean up
    free(test_component_pool.member1);
    free(test_component_pool.member2);
    free(test2_component_pool.member1);
    free(test2_component_pool.member2);
    free(test3_component_pool.member1);
    free(test3_component_pool.member2);

    entity_pool_destroy(&components_of_guid);
    assert(components_of_guid == NULL);

    entity_pool_destroy(&entity_pool);

    printf("All tests ran successfully\n");

    return 0;
}
