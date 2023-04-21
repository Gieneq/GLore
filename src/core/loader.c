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

    /* Free cJSON objects */
    if(json_items_array) {
        cJSON_Delete(json_items_array);
    }

    return RESULT_OK;
}

result_t loader_parse_player(const char *buffer, database_t* database, player_t* player) {
    printf("Parsing player data...\n");
    
    /* Convert to cJSON objects */
    cJSON* json_player_object = cJSON_Parse(file_read_buffer);
    if(!json_player_object) {
        return RESULT_ERROR;
    }

    if(!cJSON_IsObject(json_player_object)) {
        printf("Error: player.json is not an object.\n");
        return RESULT_ERROR;
    }

    char* player_name;
    stats_t player_stats;

    /* Get player name */
    if(!cJSON_HasObjectItem(json_player_object, "player_name")) {
        printf("Error: player.json has no name.\n");
        return RESULT_ERROR;
    }

    cJSON* player_name_object = cJSON_GetObjectItem(json_player_object, "player_name");

    if(!cJSON_IsString(player_name_object)) {
        printf("Error: player name is not a string.\n");
        return RESULT_ERROR;
    }

    player_name = cJSON_GetStringValue(player_name_object);

    
    /* Get player stats */
    if(!cJSON_HasObjectItem(json_player_object, "stats")) {
        printf("Error: player.json has no stats.\n");
        return RESULT_ERROR;
    }
    cJSON* player_stats_object = cJSON_GetObjectItem(json_player_object, "stats");

    if(!cJSON_HasObjectItem(player_stats_object, "exp")
        || !cJSON_HasObjectItem(player_stats_object, "health")) {
        printf("Error: player.json missing some stats.\n");
        return RESULT_ERROR;
    }
    cJSON* player_exp_object = cJSON_GetObjectItem(player_stats_object, "exp");
    cJSON* player_health_object = cJSON_GetObjectItem(player_stats_object, "health");
    
    if(!cJSON_IsNumber(player_exp_object) || !cJSON_IsNumber(player_health_object)) {
        printf("Error: player stats value error.\n");
        return RESULT_ERROR;
    }

    player_stats.experience = (int)cJSON_GetNumberValue(player_exp_object);
    player_stats.health = (int)cJSON_GetNumberValue(player_health_object);

    /* Get player questlog */
    if(!cJSON_HasObjectItem(json_player_object, "quests_log")) {
        printf("Error: player.json has no questlog.\n");
        return RESULT_ERROR;
    }
    cJSON* player_questlog_object = cJSON_GetObjectItem(json_player_object, "quests_log");
    cJSON* player_questlog_entry = NULL;
    cJSON_ArrayForEach(player_questlog_entry, player_questlog_object) {
        // printf("Questlog entry: %s\n", cJSON_Print(player_questlog_entry));

        cJSON* quest_id_object = cJSON_GetObjectItem(player_questlog_entry, "quest_id");
        if(!cJSON_IsNumber(quest_id_object)) {
            printf("Error: player questlog entry is not a valid quest id.\n");
            return RESULT_ERROR;
        }

        cJSON* quest_current_stage_object = cJSON_GetObjectItem(player_questlog_entry, "current_stage");
        if(!cJSON_IsNumber(quest_current_stage_object)) {
            printf("Error: player questlog entry is not a valid quest id.\n");
            return RESULT_ERROR;
        }

        int quest_id = (int)cJSON_GetNumberValue(quest_id_object);
        int quest_current_stage = (int)cJSON_GetNumberValue(quest_current_stage_object);

        printf("Todo: add quest %d with stage %d to player questlog.\n", quest_id, quest_current_stage); // TODO: add quest to player questlog

        // int quest_id = (int)cJSON_GetNumberValue(player_questlog_entry);
        // quest_t* quest = database_get_quest(database, quest_id);
        // if(!quest) {
        //     printf("Error: player questlog entry is not a valid quest id.\n");
        //     return RESULT_ERROR;
        // }

        // player_add_quest(player, quest);
    }

    /* Update player object with loaded data */
    player_set_name(player, player_name);
    player_set_stats(player, &player_stats);
    
    /* Free cJSON objects */
    if(json_player_object) {
        cJSON_Delete(json_player_object);
    }

    return RESULT_OK;
}

result_t loader_parse(world_t* world, database_t* database) {
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
    if(loader_from_file("../data/player.json", file_read_buffer, CORE_FILE_READ_BUFFER_SIZE) != RESULT_OK) {
        return RESULT_ERROR;
    }

    if(loader_parse_player(file_read_buffer, database, &(world->player)) != RESULT_OK) {
        return RESULT_ERROR;
    }

    /* World */

    return RESULT_OK;
}
