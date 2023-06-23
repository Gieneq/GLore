#include "container.h"
#include <stdio.h>
#include <string.h>

/* Statics */
static option_t container_get_first_free_slot(container_t* container, item_t** item) {
    for(int i = 0; i < container->capacity; i++) {
        if(container->items[i].id == INVALID_ID) {
            *item = &container->items[i];
            return OPTION_SOME;
        }
    }
    *item = NULL;
    return OPTION_NONE;
}

/* Statics - adding */
static bool_t container_can_fit_stackable_item(const container_t* container, int item_id, int items_count, int stack_size) {
    /* Case if items count exceeds capacity */
    if(items_count > container->capacity * stack_size) {
        return BOOL_FALSE;
    }

    int free_slots_count = container->capacity - container->item_count;
    
    /* Case if items can easly fit into container */
    if(items_count <= free_slots_count * stack_size) {
        return BOOL_TRUE;
    }

    /* Case if items can fit into container, 
    *  but some stacks will be partially filled.
    *  Simulate it.
    */

    /* Create temporary container */
    container_t temp_container;
    memcpy(&temp_container, container, sizeof(container_t));
    int items_to_dispose = items_count;

    /* Simulate adding items */
    for(int i = 0; i < temp_container.capacity; i++) {
        if(temp_container.items[i].id == INVALID_ID) {
            /* Free slot */
            items_to_dispose -= stack_size;
        }
        else if(temp_container.items[i].id == item_id) {
            /* Stackable item */
            int free_stackspace = stack_size - temp_container.items[i].count;
            items_to_dispose -= free_stackspace;
        }

        /* Check */
        if(items_to_dispose <= 0) {
            /* All items were added */
            return BOOL_TRUE;
        }
    }

    return BOOL_FALSE;
}

static result_t container_add_stackable_item(container_t* container, int item_id, int items_count, int stack_size) {
    /* Simulate to ensure if items will fit */
    if(container_can_fit_stackable_item(container, item_id, items_count, stack_size) == BOOL_FALSE) {
        debug_printf("Container can't fit stackable item.\n");
        return RESULT_ERROR;
    }

    /* Items can fit, displace them in container */

    /* First fill all existing stacks */
    for(int i = 0; i < container->capacity; i++) {
        if(container->items[i].id == item_id) {
            /* The same item, try filling up */
            int free_stackspace = stack_size - container->items[i].count;
            if(free_stackspace > 0) {
                /* Fillup stacks */
                int items_to_add = MIN(free_stackspace, items_count);
                container->items[i].count += items_to_add;
                items_count -= items_to_add;
            }
        }

        /* Check */
        if(items_count == 0) {
            /* All items were added */
            return RESULT_OK;
        }
    }

    /* Then dispose items to free slots */
    
    for(int i = 0; i < container->capacity; i++) {
        if(container->items[i].id == INVALID_ID) {
            /* Free slot */
            int items_to_add = MIN(stack_size, items_count);
            item_create(&container->items[i], item_id, items_to_add);
            container->item_count++;
            items_count -= items_to_add;
        }
        
        /* Check */
        if(items_count == 0) {
            /* All items were added */
            return RESULT_OK;
        }
    }
    
    /* Probably never come here */
    return RESULT_OK;
}

static result_t container_add_notstackable_item(container_t* container, int item_id) {
    item_t* free_slot = NULL;
    if(container_get_first_free_slot(container, &free_slot) == OPTION_NONE) {
        debug_printf("Container is full.\n");
        return RESULT_ERROR;
    }
    item_create(free_slot, item_id, ITEM_NOT_STACKABLE);
    container->item_count++;
    return RESULT_OK;
}

/* Statics - removal */
static bool_t container_can_remove_stackable_item(const container_t* container, int item_id, int items_count, int stack_size) {
    /* Case if items count exceeds capacity */
    if(items_count > container->capacity * stack_size) {
        return BOOL_FALSE;
    }

    int occupied_slots_count = container->item_count;
    
    /* Case if items count exceeds already occupied stacks */
    if(items_count <= occupied_slots_count * stack_size) {
        return BOOL_FALSE;
    }

    if(items_count > container_count_item(container, item_id)) {
        return BOOL_FALSE;
    }
    return BOOL_TRUE;
}

static void container_remove_empty_spaces(container_t* container) {
    for(int i = 0; i < container->capacity; i++) {
        if(container->items[i].id == INVALID_ID) {
            /* Empty slot */
            for(int j = i + 1; j < container->capacity; j++) {
                if(container->items[j].id != INVALID_ID) {
                    /* Found not empty slot */
                    container->items[i] = container->items[j];
                    container->items[j].id = INVALID_ID;
                    break;
                }
            }
        }
    }
}

static result_t container_remove_stackable_item(container_t* container, int item_id, int items_count, int stack_size) {
    if(container_can_remove_stackable_item(container, item_id, items_count, stack_size) == BOOL_FALSE) {
        debug_printf("Container can't remove stackable item.\n");
        return RESULT_ERROR;
    }

    /* Items can be removed, start removing from the end */
    for(int i = container->capacity - 1; i >= 0; i--) {
        if(container->items[i].id == item_id) {
            /* Found */
            int items_to_remove = MIN(container->items[i].count, items_count);

            if(items_to_remove == container->items[i].count) {
                /* Remove whole stack */
                item_clear(&container->items[i]);
                container->item_count--;
            }
            else {
                /* Remove part of stack */
                container->items[i].count -= items_to_remove;
            }
            items_count -= items_to_remove;
        }

        /* Check */
        if(items_count == 0) {
            /* All items were removed */
            container_remove_empty_spaces(container);
            return RESULT_OK;
        }
    }
    /* Probably never come here */
    return RESULT_OK;
}

static result_t container_remove_notstackable_item(container_t* container, int item_id) {
    /* Find first item with the id startign from the end */
    for(int i = container->capacity - 1; i >= 0; i--) {
        if(container->items[i].id == item_id) {
            /* Found */
            item_clear(&container->items[i]);
            container->item_count--;
            return RESULT_OK;
        }
    }
    debug_printf("Item not found.\n");
    return RESULT_ERROR;
}


/* Public */
result_t container_init(container_t* container, int capacity) {
    container_clear(container);
    if((capacity < 0) || (capacity > CONTAINER_MAX_CAPACITY)) {
        error_printf("Invalid container capacity: %d\n", capacity);
        return RESULT_ERROR;
    }

    container->capacity = capacity;
    return RESULT_OK;
}

void container_clear(container_t* container) {
    memset(container, 0, sizeof(container_t));
    container->capacity = 0;
    container->item_count = 0;
}

result_t container_add_item(container_t* container, const item_t* item, const item_data_t* item_data) {
    /* Requirement - item and item_data must match, should be fine in general */
    if(item->id == INVALID_ID) {
        error_printf("Invalid item id.\n");
        return RESULT_ERROR;
    }

    if(item->id != item_data->id) {
        error_printf("Item and item_data mismatch.\n");
        return RESULT_ERROR;
    }

    if(item->count <= 0 || item->count > item_data->max_stack) {
        error_printf("Invalid item count: %d\n", item->count);
        return RESULT_ERROR;
    }

    /* Stackable items */
    if(item_data_is_stackable(item_data) == BOOL_TRUE) {
        return container_add_stackable_item(container, item->id, item->count, item_data->max_stack);
    }

    /* Not stackable */
    return container_add_notstackable_item(container, item->id);
}


result_t container_remove_item_by_id(container_t* container, const item_t* item, const item_data_t* item_data) {
    /* Requirement - item and item_data must match, should be fine in general */
    if(item->id == INVALID_ID) {
        error_printf("Invalid item id.\n");
        return RESULT_ERROR;
    }

    if(item->id != item_data->id) {
        error_printf("Item and item_data mismatch.\n");
        return RESULT_ERROR;
    }

    if(item->count <= 0 || item->count > item_data->max_stack) {
        error_printf("Invalid item count: %d\n", item->count);
        return RESULT_ERROR;
    }

    /* Stackable items */
    if(item_data_is_stackable(item_data) == BOOL_TRUE) {
        return container_remove_stackable_item(container, item->id, item->count, item_data->max_stack);
    }

    /* Not stackable */
    return container_remove_notstackable_item(container, item->id);
    return RESULT_ERROR;
}

bool_t container_has_free_slot(const container_t* container) {
    return container->item_count < container->capacity ? BOOL_TRUE : BOOL_FALSE;
}

bool_t container_is_empty(const container_t* container) {
    return container->item_count == 0 ? BOOL_TRUE : BOOL_FALSE;
}

bool_t container_contains_item(const container_t* container, int id) {
    for(int i = 0; i < container->item_count; i++) {
        if(container->items[i].id == id) {
            return BOOL_TRUE;
        }
    }
    return BOOL_FALSE;
}



int container_count_item(const container_t* container, int item_id) {
    int count = 0;
    for(int i = 0; i < container->item_count; i++) {
        if(container->items[i].id == item_id) {
            count += container->items[i].count;
        }
    }
    return count;
}