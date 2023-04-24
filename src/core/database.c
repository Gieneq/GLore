#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "database.h"

static const char DEFAULT_NAME[] = "Unknown"; 

/* General */
void database_init(database_t *database) {
    items_datatable_init(&database->items_data_table);
    quests_datatable_init(&database->quests_data_table);
}

/* Items */
void items_datatable_init(items_datatable_t *items_datatable) {
    items_datatable->count = 0;
}

result_t database_save_item_data(database_t *database, const item_data_t* item_data) {
    //todo add some checks    
    const size_t free_db_index = database->items_data_table.count;
    memcpy(database->items_data_table.data + free_db_index, item_data, sizeof(item_data_t));
    printf("Saving item data: %d: %s\n",database->items_data_table.data[free_db_index].id, database->items_data_table.data[free_db_index].name);
    database->items_data_table.count++;
    return RESULT_OK;
}

size_t database_get_items_count(const database_t *database) {
    return database->items_data_table.count;
}

/* Watch out! Item data cannot be NULL */
result_t database_get_item_data_by_id(const database_t *database, const int id, item_data_t* item_data) {
    if(item_data == NULL) {
        return RESULT_ERROR;
    }

    for(size_t i = 0; i < database->items_data_table.count; i++) {
        if(database->items_data_table.data[i].id == id) {
            memcpy(item_data, database->items_data_table.data + i, sizeof(item_data_t));
            return RESULT_OK;
        }
    }

    /* Set default name */
    memcpy(item_data->name, DEFAULT_NAME, sizeof(DEFAULT_NAME));
    
    return RESULT_ERROR;
}

/* Quests */
void quests_datatable_init(quests_datatable_t *quests_datatable) {
    quests_datatable->count = 0;
}

result_t database_save_quests_data(database_t *database, const quest_data_t* quest_data) {
    //todo add some checks    
    const size_t free_db_index = database->quests_data_table.count;
    memcpy(database->quests_data_table.data + free_db_index, quest_data, sizeof(quest_data_t));
    database->quests_data_table.count++;
}

size_t database_get_quests_count(const database_t *database) {
    return database->quests_data_table.count;
}

result_t database_get_quests_data_by_id(const database_t *database, const int id, quest_data_t* quest_data) {
    if(quest_data == NULL) {
        return RESULT_ERROR;
    }

    for(size_t i = 0; i < database->quests_data_table.count; i++) {
        if(database->quests_data_table.data[i].quest_id == id) {
            memcpy(quest_data, database->quests_data_table.data + i, sizeof(quest_data_t));
            return RESULT_OK;
        }
    }

    return RESULT_ERROR;
}
