#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "item.h"


void item_data_create(item_data_t* item_data, const int item_id, const char* item_name) {
    item_data->id = item_id;
    size_t chars_count = MIN(strlen(item_name), ITEM_NAME_MAX_LENGTH);
    strncpy(item_data->name, item_name, chars_count);
}