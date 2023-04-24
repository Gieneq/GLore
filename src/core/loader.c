#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "loader.h"
#include "cJSON.h"
#include "item.h"
#include "quest.h"

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

static result_t loader_parse_player(const char *buffer, database_t* database, player_t* player) {
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

        printf("__Todo: add quest %d with stage %d to player questlog.\n", quest_id, quest_current_stage); // TODO: add quest to player questlog

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

static result_t loader_parse_quest_stages(cJSON* json_quest_stages, database_t* database, quest_data_t* quest_data) {
    /* Iterate over quest stages */
    cJSON* json_quest_stage = NULL;
    cJSON_ArrayForEach(json_quest_stage, json_quest_stages) {
        quest_stage_data_t quest_stage_data;
        quest_stage_data_create(&quest_stage_data);

        if(!cJSON_IsObject(json_quest_stage)) {
            printf("Error: quest stage is not an object.\n");
            return RESULT_ERROR;
        }

        /* Get quest stage no */
        if(!cJSON_HasObjectItem(json_quest_stage, "stage_no")) {
            printf("Error: quest stage has no id.\n");
            return RESULT_ERROR;
        }
        cJSON* json_quest_stage_no = cJSON_GetObjectItem(json_quest_stage, "stage_no");

        if(!cJSON_IsNumber(json_quest_stage_no)) {
            printf("Error: quest stage id is not a number.\n");
            return RESULT_ERROR;
        }

        quest_stage_data.stage_no = (int)cJSON_GetNumberValue(json_quest_stage_no);


        /* Get quest objective */
        if(!cJSON_HasObjectItem(json_quest_stage, "objective")) {
            printf("Error: quest stage requirements has no objective.\n");
            return RESULT_ERROR;
        }

        cJSON* json_quest_objective = cJSON_GetObjectItem(json_quest_stage, "objective");

        if(!cJSON_IsString(json_quest_objective)) {
            printf("Error: quest stage objective is not a string.\n");
            return RESULT_ERROR;
        }

        char *quest_objective = (char*)cJSON_GetStringValue(json_quest_objective);
        cpystr_trimed(quest_stage_data.objective, quest_objective, QUEST_OBJECTIVE_BUFFER_SIZE);
        printf("Quest objective: %s\n", quest_stage_data.objective);

        /* Get quest requirements */
        if(!cJSON_HasObjectItem(json_quest_stage, "requirements")) {
            printf("Error: quest stage has no requirements.\n");
            return RESULT_ERROR;
        }

        cJSON* json_quest_requirements = cJSON_GetObjectItem(json_quest_stage, "requirements");

        if(!cJSON_IsObject(json_quest_requirements)) {
            printf("Error: quest stage requirements is not an object.\n");
            return RESULT_ERROR;
        }

        /* Get quest requirements: items */
        if(!cJSON_HasObjectItem(json_quest_requirements, "items")) {
            printf("Error: quest stage requirements has no items.\n");
            return RESULT_ERROR;
        }

        cJSON* json_quest_requirements_items = cJSON_GetObjectItem(json_quest_requirements, "items");

        if(!cJSON_IsArray(json_quest_requirements_items)) {
            printf("Error: quest stage requirements items is not an array.\n");
            return RESULT_ERROR;
        }

        cJSON* json_quest_requirements_item = NULL;
        
        cJSON_ArrayForEach(json_quest_requirements_item, json_quest_requirements_items) {
            if(!cJSON_IsObject(json_quest_requirements_item)) {
                printf("Error: quest stage requirements item is not an object.\n");
                return RESULT_ERROR;
            }

            /* Get item id */
            if(!cJSON_HasObjectItem(json_quest_requirements_item, "item_id")) {
                printf("Error: quest stage requirements item has no id.\n");
                return RESULT_ERROR;
            }
            cJSON* json_quest_requirements_item_id = cJSON_GetObjectItem(json_quest_requirements_item, "item_id");

            if(!cJSON_IsNumber(json_quest_requirements_item_id)) {
                printf("Error: quest stage requirements item id is not a number.\n");
                return RESULT_ERROR;
            }

            int item_id = (int)cJSON_GetNumberValue(json_quest_requirements_item_id);

            /* Get item count */
            if(!cJSON_HasObjectItem(json_quest_requirements_item, "count")) {
                printf("Error: quest stage requirements item has no count.\n");
                return RESULT_ERROR;
            }
            cJSON* json_quest_requirements_item_count = cJSON_GetObjectItem(json_quest_requirements_item, "count");

            if(!cJSON_IsNumber(json_quest_requirements_item_count)) {
                printf("Error: quest stage requirements item count is not a number.\n");
                return RESULT_ERROR;
            }

            int item_count = (int)cJSON_GetNumberValue(json_quest_requirements_item_count);


            printf("__Todo: add item %d with count %d to quest requirements.\n", item_id, item_count); // TODO: add item to quest requirements

            quest_requirements_add_item(&(quest_stage_data.requirements), item_id, item_count);
        }

        /* Get quest returning NPC */
        if(!cJSON_HasObjectItem(json_quest_stage, "returning_npc_id")) {
            printf("Error: quest stage has no returning_npc_id.\n");
            return RESULT_ERROR;
        }

        cJSON* json_quest_stage_returning_npc_id = cJSON_GetObjectItem(json_quest_stage, "returning_npc_id");

        if(!cJSON_IsNumber(json_quest_stage_returning_npc_id)) {
            printf("Error: quest stage returning_npc_id is not a number.\n");
            return RESULT_ERROR;
        }

        quest_stage_data.returning_npc_id = (int)cJSON_GetNumberValue(json_quest_stage_returning_npc_id);


        /* Get quest rewards */
        if(!cJSON_HasObjectItem(json_quest_stage, "rewards")) {
            printf("Error: quest stage has no rewards.\n");
            return RESULT_ERROR;
        }

        cJSON* json_quest_rewards = cJSON_GetObjectItem(json_quest_stage, "rewards");

        if(!cJSON_IsObject(json_quest_rewards)) {
            printf("Error: quest stage rewards is not an object.\n");
            return RESULT_ERROR;
        }

        /* Get quest rewards: items */
        if(!cJSON_HasObjectItem(json_quest_rewards, "items")) {
            printf("Error: quest stage rewards has no items.\n");
            return RESULT_ERROR;
        }

        cJSON* json_quest_rewards_items = cJSON_GetObjectItem(json_quest_rewards, "items");

        if(!cJSON_IsArray(json_quest_rewards_items)) {
            printf("Error: quest stage rewards items is not an array.\n");
            return RESULT_ERROR;
        }

        cJSON* json_quest_rewards_item = NULL;
        cJSON_ArrayForEach(json_quest_rewards_item, json_quest_rewards_items) {
            if(!cJSON_IsObject(json_quest_rewards_item)) {
                printf("Error: quest stage rewards item is not an object.\n");
                return RESULT_ERROR;
            }

            /* Get item id */
            if(!cJSON_HasObjectItem(json_quest_rewards_item, "item_id")) {
                printf("Error: quest stage rewards item has no id.\n");
                return RESULT_ERROR;
            }
            cJSON* json_quest_rewards_item_id = cJSON_GetObjectItem(json_quest_rewards_item, "item_id");

            if(!cJSON_IsNumber(json_quest_rewards_item_id)) {
                printf("Error: quest stage rewards item id is not a number.\n");
                return RESULT_ERROR;
            }

            int item_id = (int)cJSON_GetNumberValue(json_quest_rewards_item_id);

            /* Get item count */
            if(!cJSON_HasObjectItem(json_quest_rewards_item, "count")) {
                printf("Error: quest stage rewards item has no count.\n");
                return RESULT_ERROR;
            }
            cJSON* json_quest_rewards_item_count = cJSON_GetObjectItem(json_quest_rewards_item, "count");

            if(!cJSON_IsNumber(json_quest_rewards_item_count)) {
                printf("Error: quest stage rewards item count is not a number.\n");
                return RESULT_ERROR;
            }

            int item_count = (int)cJSON_GetNumberValue(json_quest_rewards_item_count);

            quest_reward_add_item(&(quest_stage_data.reward), item_id, item_count);
        }

        /* Get quest rewards: exp */
        if(!cJSON_HasObjectItem(json_quest_rewards, "exp")) {
            printf("Error: quest stage rewards has no exp.\n");
            return RESULT_ERROR;
        }

        cJSON* json_quest_rewards_exp = cJSON_GetObjectItem(json_quest_rewards, "exp");

        if(!cJSON_IsNumber(json_quest_rewards_exp)) {
            printf("Error: quest stage rewards exp is not a number.\n");
            return RESULT_ERROR;
        }

        quest_stage_data.reward.exp = (int)cJSON_GetNumberValue(json_quest_rewards_exp);

        /* Finally add stage to quest data */
        quest_data_add_stage(quest_data, &quest_stage_data);
    }


    return RESULT_OK;
}

static result_t loader_parse_quests_data(const char *buffer, database_t* database) {
    printf("Parsing quests data...\n");
    quest_data_printf_attach_db(database);

    /* Check database requirements */
    if(!database || database_get_items_count(database) == 0) {
        printf("Error: database is not initialized or items are not loaded.\n");
        return RESULT_ERROR;
    }
    
    /* Convert to cJSON objects */
    cJSON* json_quests_data_list = cJSON_Parse(file_read_buffer);
    if(!json_quests_data_list) {
        return RESULT_ERROR;
    }

    if(!cJSON_IsArray(json_quests_data_list)) {
        printf("Error: quests.json is not an array.\n");
        return RESULT_ERROR;
    }

    /* Iterate over quests data */
    cJSON* json_quest_data = NULL;
    cJSON_ArrayForEach(json_quest_data, json_quests_data_list) {
        quest_data_t quest_data;

        if(!cJSON_IsObject(json_quest_data)) {
            printf("Error: quests.json is not an array of objects.\n");
            return RESULT_ERROR;
        }

        /* Get quest id */
        if(!cJSON_HasObjectItem(json_quest_data, "id")) {
            printf("Error: quests.json has no id.\n");
            return RESULT_ERROR;
        }
        cJSON* json_quest_id = cJSON_GetObjectItem(json_quest_data, "id");
        if(!cJSON_IsNumber(json_quest_id)) {
            printf("Error: quest id is not a number.\n");
            return RESULT_ERROR;
        }
        int quest_id = (int)cJSON_GetNumberValue(json_quest_id);

        /* Get quest name */
        if(!cJSON_HasObjectItem(json_quest_data, "name")) {
            printf("Error: quests.json has no name.\n");
            return RESULT_ERROR;
        }
        cJSON* json_quest_name = cJSON_GetObjectItem(json_quest_data, "name");
        if(!cJSON_IsString(json_quest_name)) {
            printf("Error: quest name is not a string.\n");
            return RESULT_ERROR;
        }
        char* quest_name = cJSON_GetStringValue(json_quest_name);

        /* Get quest brief */
        if(!cJSON_HasObjectItem(json_quest_data, "brief")) {
            printf("Error: quests.json has no brief.\n");
            return RESULT_ERROR;
        }
        cJSON* json_quest_brief = cJSON_GetObjectItem(json_quest_data, "brief");
        if(!cJSON_IsString(json_quest_brief)) {
            printf("Error: quest brief is not a string.\n");
            return RESULT_ERROR;
        }
        char* quest_brief = cJSON_GetStringValue(json_quest_brief);

        /* Get quest starting NPC id */
        if(!cJSON_HasObjectItem(json_quest_data, "starting_npc_id")) {
            printf("Error: quests.json has no starting_npc_id.\n");
            return RESULT_ERROR;
        }

        cJSON* json_quest_starting_npc_id = cJSON_GetObjectItem(json_quest_data, "starting_npc_id");

        if(!cJSON_IsNumber(json_quest_starting_npc_id)) {
            printf("Error: quest starting_npc_id is not a number.\n");
            return RESULT_ERROR;
        }

        int quest_starting_npc_id = (int)cJSON_GetNumberValue(json_quest_starting_npc_id);

        /* Get quest stages */
        if(!cJSON_HasObjectItem(json_quest_data, "stages")) {
            printf("Error: quests.json has no stages.\n");
            return RESULT_ERROR;
        }
        cJSON* json_quest_stages = cJSON_GetObjectItem(json_quest_data, "stages");
        if(!cJSON_IsArray(json_quest_stages)) {
            printf("Error: quest stages is not an array.\n");
            return RESULT_ERROR;
        }

        /* Create quest data */
        if(quest_data_create(&quest_data, quest_id, quest_name, quest_brief, quest_starting_npc_id) != RESULT_OK) {
            printf("Error: failed to create quest data.\n");
            return RESULT_ERROR;
        }

        /* Parse quest stages */
        if(loader_parse_quest_stages(json_quest_stages, database, &quest_data) != RESULT_OK) {
            printf("Error: failed to parse quest stages.\n");
            return RESULT_ERROR;
        }

        /* Check */
        int quest_stages_count = cJSON_GetArraySize(json_quest_stages);

        if(quest_data.stages_count != quest_stages_count) {
            printf("Error: quest stages %d count is not equal to quest stages count %d.\n", quest_data.stages_count, quest_stages_count);
            return RESULT_ERROR;
        }

        /* Finished loading one quest */
        quest_data_printf(&quest_data);
        database_save_quests_data(database, &quest_data);

    }
    
    /* Free cJSON objects */
    if(json_quests_data_list) {
        cJSON_Delete(json_quests_data_list);
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

    item_data_t some_item_data;
    database_get_item_data_by_id(database, 4, &some_item_data);
    printf("Some item: %d, %s\n", (int)(some_item_data.id), some_item_data.name);

    /* Objects */
    
    /* Quests */
    if(loader_from_file("../data/quests.json", file_read_buffer, CORE_FILE_READ_BUFFER_SIZE) != RESULT_OK) {
        return RESULT_ERROR;
    }

    if(loader_parse_quests_data(file_read_buffer, database) != RESULT_OK) {
        return RESULT_ERROR;
    }
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
