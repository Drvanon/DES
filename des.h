#include "des_internal.h"

/** Struct that creates a layer of abstraction between DES and the user's component pools. */
typedef struct
MetaComponentPool
{
    u64 *mask; /** Bitwise representation of the table. 1 means the slot/row is in use, 0 implies the row can be filled with a new entity. */
    int size; /** Size in rows that the component pool should be able to hold. This information is supplied by the user. */

    void *component_pool; /** Reference to the component pool */
} MetaComponentPool;

typedef struct
EntityPool
{
    int size; /** Amount of rows that the pool will be able to hold */
    int *guid; /** Globally Unique IDentifier of an identity. `guid[i]` represents the i-th entity. */
    MetaComponentPool **component_pool; /** List of pointers to the relevant component pool */
    int *component_index; /** Index in the component pool specified in `component_pool` that gives the value for this entity.*/
} EntityPool;


/**
 * @brief Create EntityPool and malloc it's component with the size provided, returns NULL if insufficient RAM
 *
 * @param int size
 * @return EntityPool*
 */
EntityPool *entity_pool_create(int size);

/**
 * @brief Free all memory allocated for the EntityPool.
 *
 * @param EntityPool*
 * @return void
 */
void entity_pool_destroy(EntityPool *entity_pool);

/**
 * @brief Find the index first empty row in the provided EntityPool.
 *
 * @param EntityPool*
 * @return int
 */
int entity_pool_find_empty_row(EntityPool *entity_pool);

/**
 * @brief Retrieve a new guid for an entity and register the change.
 *
 * @param EntityPool*
 * @return int guid
 */
int entity_create(EntityPool *entity_pool);

/**
 * @brief Remove all data on the entity in the provided EntityPool.
 *
 * @param EntityPool*
 * @param int guid
 * @return void
 */
void entity_remove(EntityPool *entity_pool, int guid);

/**
 * @brief Retrieve upto max_query_size components linked to the entity in an EntityPool.
 *
 * @param EntityPool*
 * @param int guid
 * @param MetaComponentPool
 * @param int max_query_size
 *
 * @return EntityPool
 */
EntityPool *entity_get_components(EntityPool *entity_pool, int guid, int max_query_size);


/**
 * @brief Register a component for use in the DES system.
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

/**
 * @brief Create a new row in the component-entity pool
 *
 * This function takes a previously established GUID and a MetaComponentPool
 * to create a new row in the component-entity pool. This is to be used
 * in combination with createEntity, after having registered your
 * component.
 * @code
 * int your_component_pool_size = 200; // Amount of rows the componentPool can have
 * EnityPool *entity_pool = entity_pool_create(2000);
 * MetaComponentPoolPool YourMetaComponentPoolPool = component_register(entity_pool, *YourComponentPool, your_component_pool_size);
 * int guid = entity_create();
 * int component_index = component_add_to_entity_ID(guid, &YourMetaComponentPoolPool);
 * YourComponentPool.member1[component_index] = "Some content";
 *
 * // And after running the program:
 * entity_pool_destroy(entity_pool);
 * free(YourComponent.member1);
 * free(YourComponent.member2); // etc.
 * @endcode
 *
 * @param EntityPool*
 * @param int guid
 * @param MetaComponentPool*
 *
 * @return int This variable refers to the index of the user defined component pool that can be set to the prefered data, see example above.
 */
int component_add_to_entity_ID(EntityPool *entity_pool, int guid, MetaComponentPool *meta_component_pool);

/**
 * @brief Add a component to an entity if the user has already obtained an empty row.
 *
 * It is possible that the user will be aware of the row
 * that the entity was created in, so this is a copy of
 * add_component_to_entity_ID, but works based of index instead
 * of id, which is much faster, for we will not have to go
 * trough the entire entity pool. An example for this might be inplace
 * replacement of a row.

 *
 * @param EntityPool* EntityPool in which to perform the action.
 * @param int index of the entity-component pool
 * @param int guid of the new entity
 * @param MetaComponentPool* MetaComponentPool of the component that is to be added to the entity
 *
 * @return int Index of component pool that can be used to store the data the user wants to store
 */
int component_add_to_entity_index(EntityPool *entity_pool, int index, int guid, MetaComponentPool* meta_component_pool);

/**
 * @brief Remove the row of the given index in the component-entity pool.
 *
 * @param EntityPool*
 * @param int index
 * @return int Error codes: 0 is okay, -1 is index out of bounds
 */
int component_remove_from_entity_index(EntityPool *entity_pool, int index);

/**
 * @brief Remove a single component from the specified entity
 *
 * @param EntityPool*
 * @param guid
 * @param MetaComponentPool
 * @return int Error codes: 0 is okay, -1 means could not find entity-component in pool.
 */
int component_remove_from_entity_ID(EntityPool *entity_pool, int guid, MetaComponentPool *meta_component_pool);



/**
 * @brief Find the first slot in the mask that has not been filled.
 *
 * @param MetaComponentPool*
 *
 * @return int component index of the open slot.
 */
int component_pool_get_open_slot(MetaComponentPool *meta_component_pool);

/**
 * @brief Find the state of a slot in a component pool
 *
 * This function will tell the user if the slot is in vacant or not.
 *
 * @param MetaComponentPool*
 * @param index
 *
 * @return int bool value
 */
int component_pool_get_slot(MetaComponentPool *meta_component_pool, int index);

/**
 * @brief Set slot state in meta component to be vacant or free
 *
 * @param MetaComponentPool* Meta of the component pool
 * @param int Index of the slot
 * @param int Desired state of the slot
 *
 * @return Error codes: 0 is okay, -1 is index out of bounds
 */
int component_pool_set_slot(MetaComponentPool *meta_component_pool, int index, int state);
