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

static result_t loader_parse_item_stack(cJSON* json_item_stack, item_stack_t *item_stack) {
    int item_id = 0;
    int item_quantity = 0;
    if(!cJSON_IsObject(json_item_stack)) {
        printf("Error: item stack is not an object.\n");
        return RESULT_ERROR;
    }

    /* Get item id */
    if(!cJSON_HasObjectItem(json_item_stack, "item_id")) {
        printf("Error: item stack has no item id.\n");
        return RESULT_ERROR;
    }
    cJSON* json_item_id = cJSON_GetObjectItem(json_item_stack, "item_id");

    if(!cJSON_IsNumber(json_item_id)) {
        printf("Error: item stack item id is not a number.\n");
        return RESULT_ERROR;
    }

    item_id = (int)cJSON_GetNumberValue(json_item_id);

    /* Get item quantity */
    if(!cJSON_HasObjectItem(json_item_stack, "count")) {
        printf("Error: item stack has no count.\n");
        return RESULT_ERROR;
    }
    cJSON* json_item_quantity = cJSON_GetObjectItem(json_item_stack, "count");

    if(!cJSON_IsNumber(json_item_quantity)) {
        printf("Error: item stack count is not a number.\n");
        return RESULT_ERROR;
    }

    item_quantity = (int)cJSON_GetNumberValue(json_item_quantity);

    /* Update item stack object with loaded data */
    item_stack_create(item_stack, item_id, item_quantity);

    return RESULT_OK;
}

static result_t loader_parse_item_stacks(cJSON* json_item_stacks, item_stack_t *item_stacks, int *item_stacks_count, const size_t max_item_stacks_count) {
    if(!cJSON_IsArray(json_item_stacks)) {
        printf("Error: item stacks is not an array.\n");
        return RESULT_ERROR;
    }

    int item_stacks_count_local = 0;
    cJSON* json_item_stack = NULL;
    cJSON_ArrayForEach(json_item_stack, json_item_stacks) {
        if(item_stacks_count_local >= max_item_stacks_count) {
            printf("Error: too many item stacks.\n");
            return RESULT_ERROR;
        }

        result_t result = loader_parse_item_stack(json_item_stack, &item_stacks[item_stacks_count_local]);
        if(result != RESULT_OK) {
            printf("Error: failed to parse item stack.\n");
            return result;
        }

        item_stacks_count_local++;
    }

    *item_stacks_count = item_stacks_count_local;

    return RESULT_OK;
}

static result_t loader_parse_quest_stage(cJSON* json_quest_stage, database_t* database, quest_stage_data_t* quest_stage_data) {
    quest_stage_data_create(quest_stage_data);

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

    quest_stage_data->stage_no = (int)cJSON_GetNumberValue(json_quest_stage_no);


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
    cpystr_trimed(quest_stage_data->objective, quest_objective, QUEST_OBJECTIVE_BUFFER_SIZE);

    /* Get quest stage requirements */
    if(!cJSON_HasObjectItem(json_quest_stage, "requirements")) {
        printf("Error: quest stage has no requirements.\n");
        return RESULT_ERROR;
    }

    cJSON* json_quest_stage_requirements = cJSON_GetObjectItem(json_quest_stage, "requirements");

    
    /* Get quest stage requirements: items */
    if(!cJSON_HasObjectItem(json_quest_stage_requirements, "items")) {
        printf("Error: quest stage requirements has no items.\n");
        return RESULT_ERROR;
    }

    cJSON* json_quest_stage_requirements_items = cJSON_GetObjectItem(json_quest_stage_requirements, "items");

    result_t result = loader_parse_item_stacks(json_quest_stage_requirements_items, quest_stage_data->requirements.items, &quest_stage_data->requirements.count, QUEST_REQUIREMENTS_ITEMS_MAX_COUNT);
    if(result != RESULT_OK) {
        printf("Error: failed to parse quest stage requirements items.\n");
        return result;
    }

    /* Get quest stage rewards */
    if(!cJSON_HasObjectItem(json_quest_stage, "rewards")) {
        printf("Error: quest stage has no reward.\n");
        return RESULT_ERROR;
    }

    cJSON* json_quest_stage_reward = cJSON_GetObjectItem(json_quest_stage, "rewards");

    /* Get quest stage rewards: items */
    if(!cJSON_HasObjectItem(json_quest_stage_reward, "items")) {
        printf("Error: quest stage reward has no items.\n");
        return RESULT_ERROR;
    }

    cJSON* json_quest_stage_reward_items = cJSON_GetObjectItem(json_quest_stage_reward, "items");

    if(!cJSON_IsArray(json_quest_stage_reward_items)) {
        printf("Error: quest stage reward items is not an array.\n");
        return RESULT_ERROR;
    }

    result = loader_parse_item_stacks(json_quest_stage_reward_items, quest_stage_data->reward.items, &quest_stage_data->reward.count, QUEST_REWARDS_ITEMS_MAX_COUNT);
    if(result != RESULT_OK) {
        printf("Error: failed to parse quest stage reward items.\n");
        return result;
    } 

    /* Get quest stage rewards: exp */
    if(!cJSON_HasObjectItem(json_quest_stage_reward, "exp")) {
        printf("Error: quest stage reward has no exp.\n");
        return RESULT_ERROR;
    }

    cJSON* json_quest_stage_reward_exp = cJSON_GetObjectItem(json_quest_stage_reward, "exp");

    if(!cJSON_IsNumber(json_quest_stage_reward_exp)) {
        printf("Error: quest stage reward exp is not a number.\n");
        return RESULT_ERROR;
    }

    quest_stage_data->reward.exp = (int)cJSON_GetNumberValue(json_quest_stage_reward_exp);
    return RESULT_OK;
}

static result_t loader_parse_quests_data(const char *buffer, database_t* database) {
    printf("Parsing quests data...\n");
    quest_data_attach_db_for_printf(database);

    quest_data_t quest_data;
    int quest_id = 0;
    char *quest_name = NULL;
    char* quest_brief = NULL;

    /* Check database requirements */
    if(!database || database_get_items_count(database) == 0) {
        printf("Error: database is not initialized or items are not loaded.\n");
        return RESULT_ERROR;
    }
    
    /* Convert to cJSON objects */
    cJSON* json_quest_data = cJSON_Parse(file_read_buffer);
    if(!json_quest_data) {
        return RESULT_ERROR;
    }

    if(!cJSON_IsObject(json_quest_data)) {
        printf("Error: quests.json is not an objects.\n");
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

    quest_id = (int)cJSON_GetNumberValue(json_quest_id);


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

    quest_name = cJSON_GetStringValue(json_quest_name);

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

    quest_brief = cJSON_GetStringValue(json_quest_brief);

    /* Create quest data */
    if(quest_data_create(&quest_data, quest_id, quest_name, quest_brief) != RESULT_OK) {
        printf("Error: failed to create quest data.\n");
        return RESULT_ERROR;
    }
    

    /* Parse quest stages */
    cJSON* json_quest_stages = cJSON_GetObjectItem(json_quest_data, "stages");

    if(!cJSON_IsArray(json_quest_stages)) {
        printf("Error: quest stages is not an array.\n");
        return RESULT_ERROR;
    }
    
    cJSON* json_quest_stage = NULL;
    cJSON_ArrayForEach(json_quest_stage, json_quest_stages) {
        quest_stage_data_t quest_stage_data;
        if(loader_parse_quest_stage(json_quest_stage, database, &quest_stage_data) != RESULT_OK) {
            printf("Error: failed to parse quest stages.\n");
            return RESULT_ERROR;
        }
        quest_data_add_stage(&quest_data, &quest_stage_data);
    }


    /* Parse quest dialogs */
    cJSON* json_quest_dialogs = cJSON_GetObjectItem(json_quest_data, "dialogs");

    if(!cJSON_IsArray(json_quest_dialogs)) {
        printf("Error: quest dialogs is not an array.\n");
        return RESULT_ERROR;
    }

    cJSON* json_quest_dialog = NULL;
    cJSON_ArrayForEach(json_quest_dialog, json_quest_dialogs) {
        // quest_dialog_data_t quest_dialog_data;
        dialog_cond_t dialog_cond;
        dialog_cond_init(&dialog_cond, DIALOG_TYPE_QUEST);
        loader_dialog_cond_from_json(&dialog_cond, DIALOG_TYPE_QUEST, json_quest_dialog);
        dialog_cond_printf(&dialog_cond);
        
        // if(loader_parse_quest_dialog(json_quest_dialog, database, &quest_dialog_data) != RESULT_OK) {
        //     printf("Error: failed to parse quest dialogs.\n");
        //     return RESULT_ERROR;
        // }
        // quest_data_add_dialog(&quest_data, &quest_dialog_data);
    }

    /* Save quest */
    database_save_quests_data(database, &quest_data);
    quest_data_printf(&quest_data);

    
    /* Free cJSON objects */
    if(json_quest_data) {
        cJSON_Delete(json_quest_data);
    }

    return RESULT_OK;
}

/* Publics */

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
    if(loader_from_file("../data/quests/quest_1.json", file_read_buffer, CORE_FILE_READ_BUFFER_SIZE) != RESULT_OK) {
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


result_t loader_keywords_list_from_json(keywords_list_t *list, const cJSON *json) {
    keywords_list_init(list);
    if (json->type != cJSON_Array) {
        printf("Error: JSON is not an array");
        return RESULT_ERROR;
    }
    int count = cJSON_GetArraySize(json);
    if (count > KEYWORDS_MAX_COUNT) {
        printf("Error: Too many keywords");
        return RESULT_ERROR;
    }
    for (int i = 0; i < count; i++) {
        cJSON *item = cJSON_GetArrayItem(json, i);
        if (item->type != cJSON_String) {
            printf("Error: JSON item is not a string");
            return RESULT_ERROR;
        }
        keyword_from_string(list->keywords[i], item->valuestring);
    }
    list->count = count;
    return RESULT_OK;
}


result_t loader_dialog_cond_from_json(dialog_cond_t *dialog_cond, const dialog_type_t dialog_type, const cJSON *json) {
    dialog_cond_init(dialog_cond, dialog_type);
    if(!cJSON_IsObject(json)) {
        printf("Error: JSON is not an object");
        return RESULT_ERROR;
    }
    
    /* Extract if case */
    if(!cJSON_HasObjectItem(json, "if")) {
        printf("Error: JSON has no if case");
        return RESULT_ERROR;
    }

    cJSON *json_if = cJSON_GetObjectItem(json, "if");

    if(!cJSON_IsObject(json_if)) {
        printf("Error: JSON if is not an object");
        return RESULT_ERROR;
    }

    /* Extract if content */
    /* It can be:
     * - dialog_stage as int
     * - keywords as array of strings
     * - questlog as int, it is rename of quest_state
     * - entering_quest_id as int, it is used to attach quest's dialog
     * - check_requirements bool
    */
    if(cJSON_HasObjectItem(json_if, "stage_no")) {
        cJSON *json_stage_no = cJSON_GetObjectItem(json_if, "stage_no");
        if(!cJSON_IsNumber(json_stage_no)) {
            printf("Error: JSON stage_no is not a number");
            return RESULT_ERROR;
        }
        dialog_cond->cond_if.dialog_stage = (int)(cJSON_GetNumberValue(json_stage_no));
    }

    if(cJSON_HasObjectItem(json_if, "keywords")) {
        cJSON *json_keywords = cJSON_GetObjectItem(json_if, "keywords");
        if(loader_keywords_list_from_json(&(dialog_cond->cond_if.keywords), json_keywords) != RESULT_OK) {
            printf("Error: failed to parse keywords");
            return RESULT_ERROR;
        }
    }

    
    /* Extract then case */
    if(!cJSON_HasObjectItem(json, "then")) {
        printf("Error: JSON has no then case");
        return RESULT_ERROR;
    }

    cJSON *json_then = cJSON_GetObjectItem(json, "then");

    if(!cJSON_IsObject(json_then)) {
        printf("Error: JSON then is not an object");
        return RESULT_ERROR;
    }

    /* Extract then content */
    /* It can be: 
     * - response as int
     * - next_dialog_state as int
     * - questlog_set as int
     * - give_rewards as bool
    */

    return RESULT_OK;
}