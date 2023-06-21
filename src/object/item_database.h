#pragma once
#include "utils.h"
#include "item.h"

#define ITEM_DATABASE_MAX_ITEMS 256

typedef struct item_database_t {
    item_data_t items_data[ITEM_DATABASE_MAX_ITEMS];
    int items_count;
} item_database_t;


void item_database_init(item_database_t* item_database);
option_t item_database_get_item_data_by_id(item_database_t* item_database, item_data_t** item_data, const int item_id);
option_t item_database_get_item_data_by_name(item_database_t* item_database, item_data_t** item_data, const char* item_name);
result_t item_database_append_item_data(item_database_t* item_database, const item_data_t* item_data);
void item_database_print_item(item_database_t* item_database, const item_data_t* item_data, const char* ending);
void item_database_print_all(item_database_t* item_database);