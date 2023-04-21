#pragma once
#include "utils.h"
#include "world.h"


#define ITEM_NAME_BUFFER_SIZE 32
#define ITEM_NAME_MAX_LENGTH (ITEM_NAME_BUFFER_SIZE-1)

typedef struct item_data_t {
    int id;
    char name[ITEM_NAME_BUFFER_SIZE];
} item_data_t;


void item_data_create(item_data_t* item_data, const int item_id, const char* item_name);