int assemblage_create();
int assemblage_row_find_empty();
int assemblage_register_component (int assemblage_id, MetaComponentPool* component);
int assemblage_remove_component(int assemblage_id, MetaComponentPool* component);
int assemblage_create_entity(EntityPool *entity_pool, int assemblage_id);
