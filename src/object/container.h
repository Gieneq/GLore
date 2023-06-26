#pragma once
#include "utils.h"
#include "item_database.h"
#include "item.h"

#define CONTAINER_MAX_CAPACITY 24

typedef struct container_t {
    int capacity;
    item_t items[CONTAINER_MAX_CAPACITY];
    int item_count;  //todo not very good name - not items, slots
} container_t;

result_t container_init(container_t* container, int capacity);
void container_clear(container_t* container);
result_t container_add_item(container_t* container, const item_t* item, const item_data_t* item_data);
result_t container_remove_item_by_id(container_t* container, const item_t* item, const item_data_t* item_data);
bool_t container_has_free_slot(const container_t* container);
bool_t container_is_empty(const container_t* container);
bool_t container_contains_item(const container_t* container, int id);
int container_count_item(const container_t* container, int item_id);