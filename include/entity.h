#ifndef ENTITY_H
#define ENTITY_H

#include "des_internal.h"

/**
 * @brief Struct that creates a layer of abstraction between DES and the user's component pools.
 *
 */
typedef struct
MetaComponentPool
{
    /** Bitwise representation of the table. 1 means the slot/row is in use, 0 implies the row can be filled with a new entity. */
    u64 *mask;
    /** Size in rows that the component pool should be able to hold. This information is supplied by the user. */
    int size;

    /** Reference to the component pool */
    void *component_pool;
} MetaComponentPool;

/**
 * @brief The pool that holds the entities and component relationships
 *
 */
typedef struct
EntityPool
{
    /** Amount of rows that the pool will be able to hold */
    int size;
    /** Globally Unique IDentifier of an identity. `guid[i]` represents the i-th entity. */
    int *guid;
    /** List of pointers to the relevant component pool */
    MetaComponentPool **component_pool;
    /** Index in the component pool specified in `component_pool` that gives the value for this entity.*/
    int *component_index;
    /** Last GUID given to an entity in this list **/
    int lastGUID;
} EntityPool;


/**
 * @brief Create EntityPool and malloc it's component with the size provided, returns NULL if insufficient RAM
 *
 * @param size
 * @return EntityPool*
 */
EntityPool *entity_pool_create(int size);

/**
 * @brief Free all memory allocated for the EntityPool.
 *
 * @param entity_pool
 */
void entity_pool_destroy(EntityPool *entity_pool);

/**
 * @brief Find the index first empty row in the provided EntityPool.
 *
 * @param entity_pool
 * @return int
 */
int entity_pool_find_empty_row(EntityPool *entity_pool);

/**
 * @brief Retrieve a new guid for an entity and register the change.
 *
 * @param entity_pool
 * @return int
 */
int entity_create(EntityPool *entity_pool);

/**
 * @brief Remove all data on the entity in the provided EntityPool.
 *
 * @param entity_pool
 * @param guid
 */
void entity_remove(EntityPool *entity_pool, int guid);

/**
 * @brief Find all entities related to a list of components
 *
 * This function will gather all entities that have atleast one
 * component of the specified types and take the union of all
 * these lists.
 *
 * @param entity_pool The entity pool in which the search will be executed
 * @param components A double pointer (pointer to a series of pointers to MetaComponentPools) of the searched components
 * @param amount_of_components The number of elements in the components list
 * @return int* A pointer to a list of guids
 */
int* components_get_all_entities(EntityPool *entity_pool, MetaComponentPool **components, int amount_of_components);

#endif // ENTITY_H
