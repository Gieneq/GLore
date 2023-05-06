#pragma once
#include "utils.h"
#include "item.h"
#include "quest.h"

#define DATABASE_ITEMS_MAX_COUNT 256
#define DATABASE_QUESTS_MAX_COUNT 64
#define DATABASE_NPCS_MAX_COUNT 64

typedef item_data_t (*item_data_by_id_t)(int item_id);

typedef struct items_datatable_t {
    item_data_t data[DATABASE_ITEMS_MAX_COUNT];
    size_t count;
} items_datatable_t;

typedef struct quests_datatable_t {
    quest_data_t data[DATABASE_QUESTS_MAX_COUNT];
    size_t count;
} quests_datatable_t;

typedef struct database_t {
    items_datatable_t items_data_table;
    quests_datatable_t quests_data_table;


} database_t;

/* General */
void database_init(database_t *database);

/* Items */
void items_datatable_init(items_datatable_t *items_datatable);
result_t database_save_item_data(database_t *database, const item_data_t* item_data);
size_t database_get_items_count(const database_t *database);
result_t database_get_item_data_by_id(const database_t *database, const int id, item_data_t* item_data);

/* Quests */
void quests_datatable_init(quests_datatable_t *quests_datatable);
result_t database_save_quests_data(database_t *database, const quest_data_t* quest_data);
size_t database_get_quests_count(const database_t *database);
result_t database_get_quests_data_by_id(const database_t *database, const int id, quest_data_t* quest_data);
