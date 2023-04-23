#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "database.h"

static const char DEFAULT_NAME[] = "Unknown"; 

result_t database_init(database_t *database) {
    database->items.count = 0;
    return RESULT_OK;
}

result_t database_save_item_data(database_t *database, const item_data_t* item_data) {
    const size_t free_db_index = database->items.count;
    memcpy(database->items.data + free_db_index, item_data, sizeof(item_data_t));
    printf("Saving item data: %d: %s\n",database->items.data[free_db_index].id, database->items.data[free_db_index].name);
    database->items.count++;
    return RESULT_OK;
}

size_t database_get_items_count(const database_t *database) {
    return database->items.count;
}

/* Watch out! Item data cannot be NULL */
result_t database_get_item_data_by_id(const database_t *database, const int id, item_data_t* item_data) {
    if(item_data == NULL) {
        return RESULT_ERROR;
    }
    for(size_t i = 0; i < database->items.count; i++) {
        if(database->items.data[i].id == id) {
            memcpy(item_data, database->items.data + i, sizeof(item_data_t));
            return RESULT_OK;
        }
    }

    /* Set default name */
    memcpy(item_data->name, DEFAULT_NAME, sizeof(DEFAULT_NAME));
    
    return RESULT_ERROR;
}