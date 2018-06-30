#ifndef ENTITY_POOL_SIZE
#define ENTITY_POOL_SIZE 100
#endif
#ifndef ENTITY_COMPONENT_LIMIT
#define ENTITY_COMPONENT_LIMIT 50
#endif

int lastGUID;
int lastComponentID;

int find_empty_row();
int createEntity();
int add_component_to_entity_ID(int guid, void* component);
void add_component_to_entity_IDX (int idx, int guid, void* component);
int remove_component_from_entity(int idx);
void remove_entity(int guid);
void get_entity_components(int guid, void* components[ENTITY_COMPONENT_LIMIT]);
