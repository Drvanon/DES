#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "des.h"

EntityPool*
entity_pool_create(int size)
{
    EntityPool *entity_pool = malloc(sizeof(EntityPool));

    if (entity_pool == NULL) {
        return NULL;
    }

    entity_pool->size = size;
    entity_pool->guid = calloc(size, sizeof(*entity_pool->guid));
    entity_pool->component_pool = malloc(size*sizeof(*entity_pool->component_pool));
    entity_pool->component_index = malloc(size*sizeof(*entity_pool->component_index));

    if ( (entity_pool->guid == NULL) || (entity_pool->component_pool == NULL)  || (entity_pool->component_index == NULL) ) {
        entity_pool_destroy(&entity_pool);
        return NULL;
    }

    return entity_pool;
}


void
entity_pool_destroy(EntityPool **entity_pool_pointer)
{
    //free((*entity_pool_pointer)->guid);
    //free((*entity_pool_pointer)->component_pool);
    //free((*entity_pool_pointer)->component_index);

    free((*entity_pool_pointer));
    *entity_pool_pointer = NULL;
}

int
entity_pool_find_empty_row(EntityPool *entity_pool)
{
    if (entity_pool == NULL) {
        return -1;
    }

    int index = 0;
    while (entity_pool->guid[index] && !(index >= entity_pool->size)) {
        index++;
    }

    if (index >= entity_pool->size) {
        return -1;
    }

    return index;
}

int
entity_create(EntityPool *entity_pool)
{
    if (entity_pool == NULL) {
        return -1;
    }

    entity_pool->lastGUID++;

    return entity_pool->lastGUID;
}

int
component_add_to_entity_ID(EntityPool *entity_pool, int guid, MetaComponentPool *meta_component_pool)
{
    if (entity_pool == NULL) {
        return -1;
    }

    int index = entity_pool_find_empty_row(entity_pool);
    int component_index = component_add_to_entity_index(entity_pool, index, guid, meta_component_pool);

    return component_index;
}

int
component_add_to_entity_index(EntityPool *entity_pool, int index, int guid, MetaComponentPool* meta_component_pool)
{
    if (entity_pool == NULL) {
        return -1;
    }

    int component_index = component_pool_get_open_slot(meta_component_pool);
    if (component_index > -1) {
        component_pool_set_slot(meta_component_pool, component_index, 1);
    }

    entity_pool->guid[index] = guid;
    entity_pool->component_pool[index] = meta_component_pool;
    entity_pool->component_index[index] = component_index;

    return component_index;
}

int
component_remove_from_entity_index(EntityPool *entity_pool, int index)
{
    if (index > entity_pool->size || entity_pool == NULL) {
        return -1;
    }
    component_pool_set_slot(entity_pool->component_pool[index], entity_pool->component_index[index], 0);

    entity_pool->guid[index] = 0;
    entity_pool->component_pool[index] = NULL;
    entity_pool->component_index[index] = 0;

    return 0;
}

int
component_remove_from_entity_ID(EntityPool *entity_pool, int guid, MetaComponentPool *meta_component_pool)
{
    if (entity_pool == NULL) {
        return -1;
    }

    int index = 0;
    while (!(guid == entity_pool->guid[index]) |
           !(entity_pool->component_pool[index] == meta_component_pool)) {
        index++;
    }

    int code = component_remove_from_entity_index(entity_pool, index);
    return code;
}

void
entity_remove(EntityPool *entity_pool, int guid)
{
    if (entity_pool == NULL) {
        return;
    }

    int index = 0;
    while (index <= entity_pool->size) {
        if (entity_pool->guid[index] == guid) {
            component_remove_from_entity_index(entity_pool, index);
        }

        index++;
    }
}

typedef struct EntityLinkedList EntityLinkedList;

struct
EntityLinkedList
{
    EntityLinkedList *next;
    int guid;
};

void
append_entity(EntityLinkedList *head, int new_guid)
{
    EntityLinkedList *tail = head;
    while (tail->next) {
        tail = tail->next;
    }

    EntityLinkedList *new_list_item = malloc(sizeof *new_list_item);
    new_list_item->next = NULL;
    new_list_item->guid = new_guid;

    tail->next = new_list_item;
}

void
remove_EntityLinkedList (EntityLinkedList *head) {
    EntityLinkedList *old_head;
    while (head) {
        old_head = head;
        head = head->next;
        free(old_head);
    }
}

int*
components_get_all_entities(EntityPool *entity_pool, MetaComponentPool **components, int amount_of_components)
{
    // For every component that is required, maintain a linked list
    // that contains all guids that have that specific component.
    EntityLinkedList* entities_components[amount_of_components];

    // Initialize the lists to only contain a -1
    for (int i=0;i<amount_of_components;i++) {
        EntityLinkedList *head = malloc(sizeof head);
        head->guid = -1;
        head->next = NULL;
        entities_components[i] = head;
    }

    // Search trough all entities for every matching component
    // and register it's guid.
    int guid;
    MetaComponentPool* component_pool;
    for (int i=0;i<entity_pool->size;i++) {
        guid = entity_pool->guid[i];
        if (guid == 0) {continue;}

        component_pool = entity_pool->component_pool[i];
        for (int j=0;j<amount_of_components;j++) {
            if (components[j] == component_pool) {
                append_entity(entities_components[j], guid);
            }
        }
    }

    // Find the smallest list of entities
    int smallest_entity_list = INT_MAX;
    int smallest_component = 0;
    for (int i=0;i<amount_of_components;i++) {
        EntityLinkedList *head = entities_components[i];
        int current_size = 0;
        while (head && current_size >= smallest_entity_list) {
            head = head->next;
            current_size++;
        }

        if (current_size < smallest_entity_list) {
            smallest_entity_list = current_size;
            smallest_component = i;
        }
    }

    int* ret_guids = malloc(smallest_entity_list * sizeof(int));
    int current_guid_index = 0;

    // Find the union of all the lists
    EntityLinkedList *head = entities_components[smallest_component];
    while (head) {
        bool in_union = true;

        for (int i=0;i<amount_of_components && in_union;i++) {
            // Make sure we are not also going trough the smallest
            // list, because that is the one we are taking as the
            // basis for our union.
            if (i == smallest_component) { continue; }

            bool found = false;
            EntityLinkedList *subhead = entities_components[i];
            while (subhead) {
                if (subhead->guid == head->guid) {
                    found = true;
                    break;
                }
                subhead = subhead->next;
            }
            in_union = found;
        }

        if (in_union) {
            ret_guids[current_guid_index] = head->guid;
            current_guid_index++;
        }

        head = head->next;
    }

    for (int i=0;i<amount_of_components;i++) {
        remove_EntityLinkedList(entities_components[i]);
    }

    return ret_guids;
}
