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

/* @brief Retrieve a new guid for an entity and register the change
 *
 * @return int guid
 */
int entity_create();

/* @brief Remove all data on the entity.
 *
 * @param guid
 * @return void
 */
void entity_remove(int guid);

/* @brief Retrieve all components linked to the entity in the provided components list.
 *
 * @param guid
 * @param MetaComponentPool
 * @param *components[]
 *
 * @return void
 */
void entity_get_components(int guid, MetaComponentPool* component_pool[ENTITY_COMPONENT_LIMIT]);


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
MetaComponentPool component_pool_register(void *component_pool, int size);

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
int component_add_to_entity_ID(int guid, MetaComponentPool *meta_component_pool);

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
int component_add_to_entity_index(int index, int guid, MetaComponentPool* meta_component_pool);

/* @brief Remove the row of the given index in the component-entity pool.
 * @param int index
 * @return int Error codes: 0 is okay, -1 is index out of bounds
 */
void component_remove_from_entity_index(int index);

/* @brief Remove a single component from the specified entity
 *
 * @param guid
 * @param MetaComponentPool
 * @return int Error codes: 0 is okay, -1 means could not find entity-component in pool.
 */
void component_remove_from_entity_ID(int guid, MetaComponentPool *meta_component_pool);



/* @brief Find the first slot in the mask that has not been filled.
 *
 * @param MetaComponentPool*
 *
 * @return int component index of the open slot.
 */
int component_pool_get_open_slot(MetaComponentPool *meta_component_pool);

/* @brief Find the state of a slot in a component pool
 *
 * This function will tell the user if the slot is in vacant or not.
 *
 * @param MetaComponentPool*
 * @param index
 *
 * @return int bool value
 */
int component_pool_get_slot(MetaComponentPool *meta_component_pool, int index);

/* @brief Set slot state in meta component to be vacant or free
 *
 * @param MetaComponentPool* Meta of the component pool
 * @param int Index of the slot
 * @param int Desired state of the slot
 *
 * @return Error codes: 0 is okay, -1 is index out of bounds
 */
int component_pool_set_slot(MetaComponentPool *meta_component_pool, int index, int state);
