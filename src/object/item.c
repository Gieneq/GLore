#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "item.h"
#include "item_database.h"


result_t item_data_create(item_data_t* item_data, const int item_id, const char* item_name, const int max_stack) {
    item_data->id = item_id;
    item_data->max_stack = max_stack;
    if(strlen(item_name) > ITEM_NAME_MAX_LENGTH) {
        printf("item_data_create: item_name too long\n");
        return RESULT_ERROR;
    }
    memcpy(item_data->name, item_name, strlen(item_name) + 1);
    return RESULT_OK;
}

bool_t item_data_is_stackable(const item_data_t* item_data) {
    return item_data->max_stack > 1 ? BOOL_TRUE : BOOL_FALSE;
}


void item_create(item_t* item, const int item_id, const size_t count) {
    item->item_id = item_id;
    item->count = count;
}
