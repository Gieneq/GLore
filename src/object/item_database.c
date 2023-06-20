#include "item_database.h"


void item_database_init(item_database_t* item_database) {
    /* INVALI_ID to all */
    memset(item_database, 0, sizeof(item_database_t));
    item_database->items_count = 0;
}



option_t item_database_get_item_data_by_id(item_database_t* item_database, item_data_t** item_data, const int item_id) {
    for(int i = 0; i < item_database->items_count; i++) {
        if(item_database->items_data[i].id == item_id) {
            *item_data = &item_database->items_data[i];
            return OPTION_SOME;
        }
    }
    return OPTION_NONE;
}

option_t item_database_get_item_data_by_name(item_database_t* item_database, item_data_t** item_data, const char* item_name) {
    for(int i = 0; i < item_database->items_count; i++) {
        if(strcmp(item_database->items_data[i].name, item_name) == 0) {
            *item_data = &item_database->items_data[i];
            return OPTION_SOME;
        }
    }
    return OPTION_NONE;
}

result_t item_database_append_item_data(item_database_t* item_database, const item_data_t* item_data) {
    if(item_database->items_count >= ITEM_DATABASE_MAX_ITEMS) {
        return RESULT_ERROR;
    }

    /* Check if id already in databse */
    if(item_database_get_item_data_by_id(item_database, NULL, item_data->id) == OPTION_SOME) {
        return RESULT_ERROR;
    }

    /* Check if name already in database */
    if(item_database_get_item_data_by_name(item_database, NULL, item_data->name) == OPTION_SOME) {
        return RESULT_ERROR;
    }
    
    memcpy(&item_database->items_data[item_database->items_count], item_data, sizeof(item_data_t));
    item_database->items_count++;
    return RESULT_OK;
}