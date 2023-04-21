#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "loader.h"
#include "cJSON.h"

#define CORE_FILE_READ_BUFFER_SIZE 1024*128
static char file_read_buffer[CORE_FILE_READ_BUFFER_SIZE];

static result_t loader_from_file(const char* filename, char *buffer, const size_t max_buffer_size) {
    FILE* file = fopen(filename, "r");
    if(!file) {
        return RESULT_ERROR;
    }
    
    memset(buffer, '\0', max_buffer_size);
    size_t read_size = fread(buffer, 1, max_buffer_size, file);

    printf("Read %llu/%llu bytes from file %s.\n", read_size, strlen(buffer), filename);
    
    if(fclose(file) != 0) {
        return RESULT_ERROR;
    }
    return RESULT_OK;
}

static result_t loader_parse_items(const char *buffer, database_t* database) {
    printf("Parsing items data...\n");

    /* Convert to cJSON objects */
    cJSON* json_items_array = cJSON_Parse(file_read_buffer);
    if(!json_items_array) {
        return RESULT_ERROR;
    }

    /* Get items array */
    if(!cJSON_IsArray(json_items_array)) {
        printf("Error: items.json is not an array.\n");
        return RESULT_ERROR;
    }

    cJSON* item = NULL;
    cJSON_ArrayForEach(item, json_items_array) {
        if(!cJSON_IsObject(item)) {
            printf("Error: item is not an object.\n");
            return RESULT_ERROR;
        }

        if(!cJSON_HasObjectItem(item, "id") && !cJSON_HasObjectItem(item, "name")) {
            printf("Error: item has no id or name.\n");
            return RESULT_ERROR;
        }

        if (!cJSON_IsNumber(cJSON_GetObjectItem(item, "id"))) {
            printf("Error: item id is not a number.\n");
            return RESULT_ERROR;
        }

        if(!cJSON_IsString(cJSON_GetObjectItem(item, "name"))) {
            printf("Error: item name is not a string.\n");
            return RESULT_ERROR;
        }

        int item_id = cJSON_GetObjectItem(item, "id")->valueint;
        char* item_name = cJSON_GetObjectItem(item, "name")->valuestring;
        
        item_data_t item_data;
        item_data_create(&item_data, item_id, item_name);
        database_save_item_data(database, &item_data);
    }

    if(json_items_array) {
        cJSON_Delete(json_items_array);
    }

    return RESULT_OK;
}

result_t loader_parse(world_t world, database_t* database) {
    printf("Parsing data files...\n");

    if(!world) {
        return RESULT_ERROR;
    }

    /* Items */
    if(loader_from_file("../data/items.json", file_read_buffer, CORE_FILE_READ_BUFFER_SIZE) != RESULT_OK) {
        return RESULT_ERROR;
    }

    if(loader_parse_items(file_read_buffer, database) != RESULT_OK) {
        return RESULT_ERROR;
    }

    printf("Parsing items data... OK! Got %llu items.\n", database_get_items_count(database));

    // item_data_t some_item_data;
    // database_get_item_data_by_id(database, 2, &some_item_data);
    // printf("%s\n\n", some_item_data.name);

    /* Objects */

    /* NPCs */

    /* Player */

    /* World */

    return RESULT_OK;
}
