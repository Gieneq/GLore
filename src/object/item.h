#pragma once
#include <stdlib.h>
#include "utils.h"


#define ITEM_NAME_BUFFER_SIZE 32
#define ITEM_NAME_MAX_LENGTH (ITEM_NAME_BUFFER_SIZE-1)

#define ITEM_NOT_STACKABLE 1   

typedef struct item_data_t {
    int id;
    char name[ITEM_NAME_BUFFER_SIZE];
    int max_stack;
} item_data_t;

typedef struct item_t {
    int item_id;
    size_t count;
} item_t;

result_t item_data_create(item_data_t* item_data, const int item_id, const char* item_name, const int max_stack);
void item_data_set_not_stackable(item_data_t* item_data);
void item_data_set_stackable(item_data_t* item_data, const int max_stack);
bool_t item_data_is_stackable(const item_data_t* item_data);

void item_create(item_t* item, const int item_id, const size_t count);