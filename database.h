#pragma once
#include "utils.h"
#include "item.h"

#define DATABASE_MAX_ITEMS_COUNT 256
#define DATABASE_MAX_OBJECTS_COUNT 256
#define DATABASE_MAX_NPCS_COUNT 256

typedef struct database_t {
    struct {
        item_data_t data[DATABASE_MAX_ITEMS_COUNT];
        size_t count;
    } items;
} database_t;

/* General */
result_t database_init(database_t *database);

/* Items */
result_t database_save_item_data(database_t *database, const item_data_t* item_data);
size_t database_get_items_count(const database_t *database);
result_t database_get_item_data_by_id(const database_t *database, const int id, item_data_t* item_data);
