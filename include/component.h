#ifndef COMPONENT_H
#define COMPONENT_H

/**
 * @brief Retrieve upto max_query_size components linked to the entity in an EntityPool.
 *
 * @param entity_pool
 * @param guid
 * @param max_query_size
 * @return EntityPool*
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
 * @param component_pool Pointer to the user-defined component_pool.
 * @param size Size of the memory allocated for the pools.
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
 * @param entity_pool
 * @param guid
 * @param meta_component_pool
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
 * @param entity_pool EntityPool in which to perform the action.
 * @param index index of the entity-component pool
 * @param guid guid of the new entity
 * @param meta_component_pool MetaComponentPool of the component that is to be added to the entity
 * @return int Index of component pool that can be used to store the data the user wants to store
 */
int component_add_to_entity_index(EntityPool *entity_pool, int index, int guid, MetaComponentPool* meta_component_pool);

/**
 * @brief Remove the row of the given index in the component-entity pool.
 *
 * @param entity_pool
 * @param index
 * @return int Error codes: 0 is okay, -1 is index out of bounds
 */
int component_remove_from_entity_index(EntityPool *entity_pool, int index);

/**
 * @brief Remove a single component from the specified entity
 *
 * @param entity_pool
 * @param guid
 * @param meta_component_pool
 * @return int Error codes: 0 is okay, -1 means could not find entity-component in pool.
 */
int component_remove_from_entity_ID(EntityPool *entity_pool, int guid, MetaComponentPool *meta_component_pool);

/**
 * @brief Find the first slot in the mask that has not been filled.
 *
 * @param meta_component_pool
 *
 * @return int component index of the open slot.
 */
int component_pool_get_open_slot(MetaComponentPool *meta_component_pool);

/**
 * @brief Find the state of a slot in a component pool
 *
 * This function will tell the user if the slot is in vacant or not.
 * vacant: 0
 * occupied: 1
 *
 * @param meta_component_pool
 * @param index
 * @return int bool value
 */
int component_pool_get_slot(MetaComponentPool *meta_component_pool, int index);

/**
 * @brief Set slot state in meta component to be vacant (0) or occupied (1).
 *
 * @param meta_component_pool Meta of the component pool
 * @param index Index of the slot
 * @param state Desired state of the slot
 * @return int Error codes: 0 is okay, -1 is index out of bounds
 */
int component_pool_set_slot(MetaComponentPool *meta_component_pool, int index, int state);

#endif // DES_H
