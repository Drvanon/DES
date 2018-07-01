#include "int.h"

#ifndef ENTITY_POOL_SIZE
#define ENTITY_POOL_SIZE 100
#endif
#ifndef ENTITY_COMPONENT_LIMIT
#define ENTITY_COMPONENT_LIMIT 50
#endif

typedef struct
metaComponent {
    void *component;
    u64 *mask;
    int size;
} metaComponent;

int lastGUID;
int lastComponentID;
int find_empty_row ();

int createEntity ();

int add_component_to_entity_ID (int guid, metaComponent *metComp);

/* It is very possible that the user will be aware of the row
   that the entity was created in, so this is a copy of
   add_component_to_entity_ID, but works based of index instead
   of id, which is much faster, for we will not have to go
   trough the entire entity pool. */
int add_component_to_entity_IDX (int idx, int guid, metaComponent* metComp);

int remove_entity_component_index (int idx);

int remove_component_from_entity (int guid, metaComponent *metComp);

void remove_entity (int guid);

void get_entity_components (int guid, metaComponent* components[ENTITY_COMPONENT_LIMIT]);

metaComponent register_component (void *component, int size);

int get_open_slot_in_component (metaComponent *metComp);

/* Find the state of a slot in a component */
int get_slot_in_component (metaComponent *metComp, int index);

int set_slot_in_component (metaComponent *metComp, int index, int state);

