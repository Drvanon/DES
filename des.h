#include "des_internal.h"

#ifndef ENTITY_POOL_SIZE
#define ENTITY_POOL_SIZE 100
#endif
#ifndef ENTITY_COMPONENT_LIMIT
#define ENTITY_COMPONENT_LIMIT 50
#endif

typedef struct
MetaComponentPool 
{
    u64 *mask;
    int size;

    void *component_pool;
} MetaComponentPool;

int entity_pool_find_empty_row();
int entity_create();
void entity_remove(int guid);
void entity_get_components(int guid, MetaComponentPool* component_pool[ENTITY_COMPONENT_LIMIT]);

MetaComponentPool component_pool_register(void *component_pool, int size);

int component_add_to_entity_ID(int guid, MetaComponentPool *meta_component_pool);
int component_add_to_entity_index(int index, int guid, MetaComponentPool* meta_component_pool);

void component_remove_from_entity_index(int index);
void component_remove_from_entity_ID(int guid, MetaComponentPool *meta_component_pool);

int component_pool_get_open_slot(MetaComponentPool *meta_component_pool);
int component_pool_get_slot(MetaComponentPool *meta_component_pool, int index);
int component_pool_set_slot(MetaComponentPool *meta_component_pool, int index, int state);