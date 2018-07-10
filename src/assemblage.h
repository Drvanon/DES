/**
 * @brief Create a new assemblage.
 * 
 * @return int The new assemblage_id
 */
int assemblage_create();

/**
 * @brief Find an empty row in the assemblage table.
 *
 * @return int Index of the empty row.
 */
int assemblage_row_find_empty();

/**
 * @brief Create a entry in the assemablage table.
 * 
 * @param assemblage_id The assemblage id to which you wish to add the component
 * @param component Pointer to the component pool that will be added to the newly created entity
 * @return int Index that represents the row where the entry lives.
 */
int assemblage_register_component(int assemblage_id, MetaComponentPool* component);

/**
 * @brief Remove a component from an assemblage.
 * 
 * @param assemblage_id 
 * @param component 
 * @return int Error codes: 0 if okay, -1 if assemblage_id out not in assemblage table.
 */
int assemblage_remove_component(int assemblage_id, MetaComponentPool* component);

/**
 * @brief Create a new entity in the provided EntityPool from the assemblage that is identified by the assemblage_id.
 * 
 * @param entity_pool 
 * @param assemblage_id 
 * @return int GUID of the newly created entity
 */
int assemblage_create_entity(EntityPool *entity_pool, int assemblage_id);
