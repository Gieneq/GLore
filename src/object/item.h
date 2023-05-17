#pragma once
#include <stdlib.h>
#include "utils.h"


#define ITEM_NAME_BUFFER_SIZE 32
#define ITEM_NAME_MAX_LENGTH (ITEM_NAME_BUFFER_SIZE-1)

typedef struct item_stack {
    int item_id;
    size_t count;
} item_stack_t;

typedef struct item_data_t {
    int id;
    char name[ITEM_NAME_BUFFER_SIZE];
} item_data_t;


void item_data_create(item_data_t* item_data, const int item_id, const char* item_name);
void item_stack_create(item_stack_t* item_stack, const int item_id, const size_t count);