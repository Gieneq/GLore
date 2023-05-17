#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "item.h"


void item_data_create(item_data_t* item_data, const int item_id, const char* item_name) {
    item_data->id = item_id;
    size_t chars_count = MIN(strlen(item_name), ITEM_NAME_MAX_LENGTH);
    strncpy(item_data->name, item_name, chars_count);
}

void item_stack_create(item_stack_t* item_stack, const int item_id, const size_t count) {
    item_stack->item_id = item_id;
    item_stack->count = count;
}