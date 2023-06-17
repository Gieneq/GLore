#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "loader.h"
#include "cJSON.h"
#include "room.h"
#include "npc.h"

#define LOADER_VOIDROOM 1
#define LOADER_INITIAL_PLAYER_NAME "#Nonameplayer"

#define LOADER_RESOURCE_PATH_BUFFER_SIZE 128

#define LOADER_RESOURCES_DIR "./res"
#define LOADER_RESOURCES_WORLD_PATH (LOADER_RESOURCES_DIR "/world.json")
#define LOADER_RESOURCES_PLAYER_PATH (LOADER_RESOURCES_DIR "/player.json")
#define LOADER_RESOURCES_NPCS_DIR (LOADER_RESOURCES_DIR "/npcs")
#define LOADER_RESOURCES_QUESTS_DIR (LOADER_RESOURCES_DIR "/quests")

#define CORE_FILE_READ_BUFFER_SIZE 1024*32
static char file_read_buffer[CORE_FILE_READ_BUFFER_SIZE];

typedef struct loader_context_t {
    int npc_to_talk_to_id;
    int quest_id;
    int quest_stage;
} loader_context_t;

/* Static PFP */
static result_t loader_from_file(const char* filename, char *buffer, const size_t max_buffer_size);
static result_t loader_get_cond_if_from_cJSON(const cJSON* json, dialog_cond_if_t* cond_if, void* loader_contex);
static result_t loader_get_cond_then_from_cJSON(const cJSON* json, dialog_cond_then_t* cond_then, void* loader_contex);
static result_t loader_get_dialog_block_from_cJSON(const cJSON* json, dialog_block_t* dialog_block, void* loader_contex);
static result_t loader_load_npc_with_id(world_t* world, npc_t* npc_data, int npc_id);
static result_t loader_load_quest_data_from_cJSON(const cJSON* json, world_t* world, player_t* player, const int quest_id);

static option_t loader_get_int_from_cJSON(const cJSON* json, const char* key, int* value);
static option_t loader_get_string_from_cJSON(const cJSON* json, const char* key, char* buffer, const int buffer_size);


/**
 * Generic functions to parse JSON
*/
static option_t loader_get_int_from_cJSON(const cJSON* json, const char* key, int* value) {
    cJSON* int_json = cJSON_GetObjectItem(json, key);
    if(!int_json) {
        return OPTION_NONE;
    }

    if(!cJSON_IsNumber(int_json)) {
        return OPTION_NONE;
    }

    *value = (int)cJSON_GetNumberValue(int_json);
    return OPTION_SOME;
}

static option_t loader_get_string_from_cJSON(const cJSON* json, const char* key, char* buffer, const int buffer_size) {
    cJSON* string_json = cJSON_GetObjectItem(json, key);
    if(!string_json) {
        return OPTION_NONE;
    }

    if(!cJSON_IsString(string_json)) {
        return OPTION_NONE;
    }

    char* string_ptr = cJSON_GetStringValue(string_json);
    const int string_len = strlen(string_ptr);
    if(string_len >= buffer_size - 1) {
        return OPTION_NONE;
    }
    
    memset(buffer, '\0', buffer_size);
    memcpy(buffer, string_ptr, string_len);
    return OPTION_SOME;
}

/* Static definitions */
static result_t loader_from_file(const char* filename, char *buffer, const size_t max_buffer_size) {
    FILE* file = fopen(filename, "r");
    if(!file) {
        return RESULT_ERROR;
    }
    
    memset(buffer, '\0', max_buffer_size);
    size_t read_size = fread(buffer, 1, max_buffer_size, file);

    debug_printf("Read %llu/%llu bytes from file %s.\n", read_size, strlen(buffer), filename);
    
    if(fclose(file) != 0) {
        return RESULT_ERROR;
    }
    return RESULT_OK;
}

static result_t loader_get_cond_if_from_cJSON(const cJSON* if_case_json, dialog_cond_if_t* cond_if, void* loader_contex) {
    dialog_cond_if_init(cond_if);
    //todo in next stage, if it work, replace with helper function
    /* From stage */
    cJSON* if_from_stage_json = cJSON_GetObjectItem(if_case_json, "from_stage");
    if(!if_from_stage_json) {
        error_printf("Error: if_case missing from_stage.\n");
        return RESULT_ERROR;
    }

    if(!cJSON_IsNumber(if_from_stage_json)) {
        error_printf("Error: from_stage is not a number.\n");
        return RESULT_ERROR;
    }

    cond_if->dialog_stage = (int)cJSON_GetNumberValue(if_from_stage_json);


    /* Questlog requirement - optional */
    cJSON* if_questlog_json = cJSON_GetObjectItem(if_case_json, "with_questlog");
    if(if_questlog_json) {
        cJSON* questlog_id_json = cJSON_GetObjectItem(if_questlog_json, "id");
        cJSON* questlog_at_stage_json = cJSON_GetObjectItem(if_questlog_json, "at_stage");
        if(!questlog_id_json || !questlog_at_stage_json) {
            error_printf("Error: with_questlog missing id or at_stage.\n");
            return RESULT_ERROR;
        }

        if(!cJSON_IsNumber(questlog_id_json) || !cJSON_IsNumber(questlog_at_stage_json)) {
            error_printf("Error: with_questlog: id or at_stage not number.\n");
            return RESULT_ERROR;
        }

        cond_if->quest_stage.quest_id = (int)cJSON_GetNumberValue(questlog_id_json);
        cond_if->quest_stage.stage = (int)cJSON_GetNumberValue(questlog_at_stage_json);
        debug_printf("Q:Found questlog_requirement: %d to stage %d\n", cond_if->quest_stage.quest_id, cond_if->quest_stage.stage);
    }

    /* Questlog requirement from context - optional */
    if(loader_contex) {
        loader_context_t *context = (loader_context_t*)loader_contex;
        cond_if->quest_stage.quest_id = context->quest_id;
        cond_if->quest_stage.stage = context->quest_stage;
    }

    /* Keywords */
    cJSON* if_keywords_json = cJSON_GetObjectItem(if_case_json, "with_keywords");

    if(if_keywords_json) {
        if(!cJSON_IsArray(if_keywords_json)) {
            error_printf("Error: keywords is not an array.\n");
            return RESULT_ERROR;
        }

        /* Iterate over list of keywords and load them from resource files */
        cJSON* keyword_json = NULL;
        cJSON_ArrayForEach(keyword_json, if_keywords_json) {
            if(!cJSON_IsString(keyword_json)) {
                error_printf("Error: keyword is not a string.\n");
                return RESULT_ERROR;
            }
            const char* keyword_to_load = cJSON_GetStringValue(keyword_json);
            if(keywords_list_append(&cond_if->keywords, keyword_to_load) != RESULT_OK) {
                error_printf("Error: couldnt append keyword.\n");
                return RESULT_ERROR;
            }
        }
    } else {
        //todo swap if to negation
        return RESULT_ERROR;
    }

    return RESULT_OK;
} 

static result_t loader_get_cond_then_from_cJSON(const cJSON* then_case_json, dialog_cond_then_t* cond_then, void* loader_context) {
    dialog_cond_then_init(cond_then);
    /* Next stage */
    cJSON* then_next_stage_json = cJSON_GetObjectItem(then_case_json, "next_stage");

    if(!then_next_stage_json) {
        error_printf("Error: then_case missing next_stage.\n");
        return RESULT_ERROR;
    }

    if(!cJSON_IsNumber(then_next_stage_json)) {
        error_printf("Error: next_stage is not a number.\n");
        return RESULT_ERROR;
    }

    cond_then->next_dialog_stage = (int)cJSON_GetNumberValue(then_next_stage_json);

    /* Response */
    cJSON* then_response_json = cJSON_GetObjectItem(then_case_json, "response");

    if(then_response_json) {
        if(!cJSON_IsString(then_response_json)) {
            error_printf("Error: response is not a string.\n");
            return RESULT_ERROR;
        }
        const char* response_to_load = cJSON_GetStringValue(then_response_json);
        if(response_from_string(&cond_then->response, response_to_load, BOOL_FALSE) != RESULT_OK) {
            error_printf("Error: couldnt append response.\n");
            return RESULT_ERROR;
        }
    } else {
        /* No response */
        if(response_from_string(&cond_then->response, "", BOOL_FALSE) != RESULT_OK) {
            error_printf("Error: couldnt append response.\n");
            return RESULT_ERROR;
        }
    }

    
    /* Questlog update - optional */
    int quest_stage;
    if(loader_get_int_from_cJSON(then_case_json, "update_questlog_stage", &quest_stage) == OPTION_SOME) {
        /* Those must be included - progress in quest */
        if(!loader_context) {
            error_printf("Error: loader_context is NULL.\n");
            return RESULT_ERROR;
        }

        loader_context_t *context = (loader_context_t*)loader_context;
        if(context->quest_id == INVALID_ID) {
            error_printf("Error: quest_id is invalid.\n");
            return RESULT_ERROR;
        }

        /* Combine quest id with quest stage */
        cond_then->update_quest_stage.quest_id = context->quest_id;
        cond_then->update_quest_stage.stage = quest_stage;
        printf("Q:Found questlog_update: %d to stage %d\n", cond_then->update_quest_stage.quest_id, cond_then->update_quest_stage.stage);
    }

    return RESULT_OK;
} 

static result_t loader_get_dialog_block_from_cJSON(const cJSON* json, dialog_block_t* dialog_block, void* loader_contex) {
    if(!cJSON_IsObject(json)) {
        error_printf("Error: dialog_block is not an object.\n");
        return RESULT_ERROR;
    }

    /* Get (if) case */
    cJSON* if_case_json = cJSON_GetObjectItem(json, "if_case");
    if(!if_case_json) {
        error_printf("Error: if_case not in dialog_block.\n");
        return RESULT_ERROR;
    }

    dialog_cond_if_t loaded_cond_if;
    if(loader_get_cond_if_from_cJSON(if_case_json, &loaded_cond_if, loader_contex) == RESULT_ERROR) {
        error_printf("Error: if_case corrupted.\n");
        return RESULT_ERROR;
    }
    memcpy(&dialog_block->cond_if, &loaded_cond_if, sizeof(dialog_cond_if_t));


    /* Get (then) case */
    cJSON* then_case_json = cJSON_GetObjectItem(json, "then_case");
    if(!then_case_json) {
        error_printf("Error: then_case not in dialog_block.\n");
        return RESULT_ERROR;
    }

    dialog_cond_then_t loader_cond_then;
    if(loader_get_cond_then_from_cJSON(then_case_json, &loader_cond_then, loader_contex) == RESULT_ERROR) {
        error_printf("Error: then_case corrupted.\n");
        return RESULT_ERROR;
    }
    memcpy(&dialog_block->cond_then, &loader_cond_then, sizeof(dialog_cond_then_t));

    return RESULT_OK;
}

/* Loading player data - requires build world to set initial room */
result_t loader_load_player(world_t* world, player_t* player) {
    debug_printf("Loading player\n");

    if(!world) {
        error_printf("Error: world NULL.\n");
        return RESULT_ERROR;
    }

    if(!player) {
        error_printf("Error: player NULL.\n");
        return RESULT_ERROR;
    }

    /* Initial presets for player */
    player_set_name(player, LOADER_INITIAL_PLAYER_NAME);

    if(loader_from_file(LOADER_RESOURCES_PLAYER_PATH, file_read_buffer, CORE_FILE_READ_BUFFER_SIZE) != RESULT_OK) {
        error_printf("Error: player file not loaded.\n");
        return RESULT_ERROR;
    }
    
    /* Store player data from json files */
    /* Convert to cJSON objects */
    cJSON* json_player = cJSON_Parse(file_read_buffer);
    if(!json_player) {
        error_printf("Error: creating cJSON failed.\n");
        return RESULT_ERROR;
    }
    
    /* Get player name */
    {
        cJSON* player_name_json = cJSON_GetObjectItem(json_player, "name");
        if(!player_name_json) {
            error_printf("Error: player missing name.\n");
            return RESULT_ERROR;
        }

        if(!cJSON_IsString(player_name_json)) {
            error_printf("Error: player name is not a string.\n");
            return RESULT_ERROR;
        }
        const char* player_name = cJSON_GetStringValue(player_name_json);
        player_set_name(player, player_name);
    }

    /* Get current room id */
    {
        cJSON* current_room_id_json = cJSON_GetObjectItem(json_player, "current_room_id");
        if(!current_room_id_json) {
            error_printf("Error: player missing current_room_id.\n");
            return RESULT_ERROR;
        }

        if(!cJSON_IsNumber(current_room_id_json)) {
            error_printf("Error: current_room_id is not a number.\n");
            return RESULT_ERROR;
        }

        int current_room_id = (int)cJSON_GetNumberValue(current_room_id_json);
        if(current_room_id == INVALID_ID) {
            error_printf("Error: passed invalid id.\n");
            return RESULT_ERROR;
        }
        player->current_room_id = current_room_id;
    }

    /* Conversation - no conversation */
    cJSON_Delete(json_player);
    player->current_conversation_npc_id = INVALID_ID;
    return RESULT_OK;
}

result_t loader_load_world(world_t* world) {
    debug_printf("Loading world\n");

    if(!world) {
        error_printf("Error: world NULL.\n");
        return RESULT_ERROR;
    }

    if(loader_from_file(LOADER_RESOURCES_WORLD_PATH, file_read_buffer, CORE_FILE_READ_BUFFER_SIZE) != RESULT_OK) {
        error_printf("Error: world file not loaded.\n");
        return RESULT_ERROR;
    }

    /* Extract rooms */
    /* Convert to cJSON objects */
    cJSON* json_rooms_array = cJSON_Parse(file_read_buffer);
    if(!json_rooms_array) {
        error_printf("Error: creating cJSON failed.\n");
        return RESULT_ERROR;
    }

    /* Get items array */
    if(!cJSON_IsArray(json_rooms_array)) {
        error_printf("Error: world.json is not an array.\n");
        return RESULT_ERROR;
    }

    /* Iterate over list of rooms */
    cJSON* room_json = NULL;
    cJSON_ArrayForEach(room_json, json_rooms_array) {
        if(!cJSON_IsObject(room_json)) {
            error_printf("Error: room_json is not an object.\n");
            return RESULT_ERROR;
        }

        /* Fill room data */
        room_t room_data;
        room_init(&room_data);

        /* Get room id */
        {
            cJSON* room_id_json = cJSON_GetObjectItem(room_json, "id");
            if(!room_id_json) {
                error_printf("Error: room missing id.\n");
                return RESULT_ERROR;
            }

            if(!cJSON_IsNumber(room_id_json)) {
                error_printf("Error: room id is not a number.\n");
                return RESULT_ERROR;
            }

            int room_id = (int)(cJSON_GetNumberValue(room_id_json));
            if (room_id == INVALID_ID) {
                error_printf("Error: passed invalid id.\n");
                return RESULT_ERROR;
            }

            /* Validate id - should be unique */
            if(world_has_room_with_id(world, room_id) == OPTION_SOME) {
                error_printf("Error: room id %d already used. Check data file!\n", room_id);
                return RESULT_ERROR;
            }

            room_data.id = room_id;
        }

        /* Get room name */
        {
            cJSON* room_name_json = cJSON_GetObjectItem(room_json, "name");
            if(!room_name_json) {
                printf("Error: room missing name.\n");
                return RESULT_ERROR;
            }

            if(!cJSON_IsString(room_name_json)) {
                printf("Error: room name is not a string.\n");
                return RESULT_ERROR;
            }

            if(room_set_name(&room_data, cJSON_GetStringValue(room_name_json)) != RESULT_OK) {
                printf("Error: cannot set room name.\n");
                return RESULT_ERROR;
            }
        }

        /* Get room description */
        {
            cJSON* room_descr_json = cJSON_GetObjectItem(room_json, "description");
            if(!room_descr_json) {
                printf("Error: room missing description.\n");
                return RESULT_ERROR;
            }

            if(!cJSON_IsString(room_descr_json)) {
                printf("Error: room description is not a string.\n");
                return RESULT_ERROR;
            }

            // todo
            // if(room_set_description(&room_data, cJSON_GetStringValue(room_descr_json)) != RESULT_OK) {
            //     printf("Error: cannot set room description.\n");
            //     return RESULT_ERROR;
            // }
        }

        /* Get optional NPC ids */
        {
            cJSON* room_npcs_json = cJSON_GetObjectItem(room_json, "npcs");
            if(room_npcs_json) {

                if(!cJSON_IsArray(room_npcs_json)) {
                    error_printf("Error: npcs is not an array.\n");
                    return RESULT_ERROR;
                }

                /* Iterate over list of npcs ids and load them from resource files */
                cJSON* npc_id_json = NULL;
                cJSON_ArrayForEach(npc_id_json, room_npcs_json) {
                    if(!cJSON_IsNumber(npc_id_json)) {
                        error_printf("Error: npc id is not a number.\n");
                        return RESULT_ERROR;
                    }
                    int npc_id_to_load = cJSON_GetNumberValue(npc_id_json);
                    
                    /* Load NPC data from file */
                    npc_t new_npc_data;
                    if(npc_init(&new_npc_data) != RESULT_OK) {
                        error_printf("Error: couldnt initialized NPC data.\n");
                        return RESULT_ERROR;
                    }

                    if(loader_load_npc_with_id(world, &new_npc_data, npc_id_to_load) != RESULT_OK) {
                        error_printf("Error: couldnt load npc data from file using id %d.\n", npc_id_to_load);
                        return RESULT_ERROR;
                    }

                    /* Store NPC data to world */
                    if(world_append_npc(world, &new_npc_data) != RESULT_OK) {
                        error_printf("Error: couldnt store NPC data to the world.\n");
                        return RESULT_ERROR;
                    }

                    /* Store NPC id to currently processed room */
                    if(room_append_npc_id(&room_data, npc_id_to_load) != RESULT_OK) {
                        error_printf("Error: couldnt store NPC id to the room.\n");
                        return RESULT_ERROR;
                    }
                }
            }
        }

        /* Get optional item stacks */
        /* Get optional roads */
        {
            cJSON* room_roads_json = cJSON_GetObjectItem(room_json, "roads");
            if(room_roads_json) {
                /* Room has some directional roads. Check all connections  */
                /* at the end of loading in case of ids of not existing rooms. */

                if(!cJSON_IsArray(room_roads_json)) {
                    error_printf("Error: roads is not an array.\n");
                    return RESULT_ERROR;
                }

                /* Iterate over list of roads (target room ids) */
                cJSON* directional_road_json = NULL;
                cJSON_ArrayForEach(directional_road_json, room_roads_json) {
                    if(!cJSON_IsNumber(directional_road_json)) {
                        error_printf("Error: road is not a number (id).\n");
                        return RESULT_ERROR;
                    }
                    int road_terget_id = cJSON_GetNumberValue(directional_road_json);
                    room_append_adjecent_room(&room_data, road_terget_id);
                }

            }
        }
        world_append_room(world, &room_data);
    }



    /* Validate all connections - should exist */
    room_t* checked_room = NULL;
    room_iter_t iter = world_get_room_iter(world);
    iterator_foreach(&checked_room, &iter) {
        /* Iterate over all adjecent rooms, check if their roads */
        /* leads somewhere. Cannot lead to the same room. */
        for(int i=0; i<checked_room->adjecent_rooms_count; i++) {
            int adjecent_room_id = checked_room->adjecent_rooms_ids[i];
            if(world_has_room_with_id(world, adjecent_room_id) != OPTION_SOME) {
                error_printf("Error: roads from %d leads nowhere %d.\n", checked_room->id, adjecent_room_id);
                return RESULT_ERROR;
            }
        }
    }


    cJSON_Delete(json_rooms_array);
    return RESULT_OK;
}

/* Loading npcs data - requires build world to set initial room */
static result_t loader_load_npc_with_id(world_t* world, npc_t* npc_data, int npc_id) {
    debug_printf("Loading npcs\n");
    if(!npc_data) {
        error_printf("Error: npc data NULL.\n");
        return RESULT_ERROR;
    }
    
    if(!world) {
        error_printf("Error: world NULL.\n");
        return RESULT_ERROR;
    }

    if(npc_id == INVALID_ID){
        error_printf("Error: NPC id invalid.\n");
        return RESULT_ERROR;
    }

    char npc_data_path[LOADER_RESOURCE_PATH_BUFFER_SIZE];
    memset(npc_data_path, '\0', LOADER_RESOURCE_PATH_BUFFER_SIZE * sizeof(char));
    sprintf(npc_data_path, "%s/npc_%d.json", LOADER_RESOURCES_NPCS_DIR, npc_id);

    if(loader_from_file(npc_data_path, file_read_buffer, CORE_FILE_READ_BUFFER_SIZE) != RESULT_OK) {
        error_printf("Error: npc %d file not loaded from %s.\n", npc_id, npc_data_path);
        return RESULT_ERROR;
    }

    /* Extract rooms */
    /* Convert to cJSON objects */
    cJSON* json_npc = cJSON_Parse(file_read_buffer);
    if(!json_npc) {
        error_printf("Error: creating cJSON failed.\n");
        return RESULT_ERROR;
    }

    /* Get npc id */
    {
        cJSON* current_room_id_json = cJSON_GetObjectItem(json_npc, "id");
        if(!current_room_id_json) {
            error_printf("Error: npc missing id.\n");
            return RESULT_ERROR;
        }

        if(!cJSON_IsNumber(current_room_id_json)) {
            error_printf("Error: npc's id is not a number.\n");
            return RESULT_ERROR;
        }

        int npc_id_from_file = (int)cJSON_GetNumberValue(current_room_id_json);
        if(npc_id_from_file != npc_id) {
            error_printf("Error: npc's id from file %d and from source %d not equal!\n", npc_id_from_file, npc_id);
            return RESULT_ERROR;
        }

        npc_data->id = npc_id;
    }

    /* Get npc name */
    {
        cJSON* npc_name_json = cJSON_GetObjectItem(json_npc, "name");
        if(!npc_name_json) {
            error_printf("Error: npc missing name.\n");
            return RESULT_ERROR;
        }

        if(!cJSON_IsString(npc_name_json)) {
            error_printf("Error: npc name is not a string.\n");
            return RESULT_ERROR;
        }

        const char* npc_name = cJSON_GetStringValue(npc_name_json);
        if(npc_set_name(npc_data, npc_name) != RESULT_OK) {
            error_printf("Error: couldnt set npc name.\n");
            return RESULT_ERROR;
        }
    }

    /* Get optional dialog blocks */
    {
        cJSON* dialog_blocks_list_json = cJSON_GetObjectItem(json_npc, "dialog_blocks");
        if(dialog_blocks_list_json) {

            if(!cJSON_IsArray(dialog_blocks_list_json)) {
                error_printf("Error: dialog_blocks is not an array.\n");
                return RESULT_ERROR;
            }

            /* Iterate over list of npcs ids and load them from resource files */
            cJSON* dialog_block_json = NULL;
            cJSON_ArrayForEach(dialog_block_json, dialog_blocks_list_json) {
                /* Get dialog block */
                dialog_block_t dialog_block;
                dialog_block_init(&dialog_block, DIALOG_TYPE_REGULAR);
                if(loader_get_dialog_block_from_cJSON(dialog_block_json, &dialog_block, NULL) != RESULT_OK) {
                    error_printf("Error: couldnt load dialog block from cJSON.\n");
                    return RESULT_ERROR;
                }

                /* Append dialog block */
                if(npc_append_dialog_block(npc_data, &dialog_block) != RESULT_OK) {
                    /* BTW appending leaves variable invalid */
                    error_printf("Cannot add dialog block to NPC.\n");
                    return RESULT_ERROR;
                }
            }
        }
    }

    cJSON_Delete(json_npc);
    /* Data should be process (saved) outside this function */
    return RESULT_OK;
}


/**
 * After loading world (especially NPCs) and player. 
 * Populate NPC's dialogs and initializate player's questlog
*/
result_t loader_load_quests(world_t* world, player_t* player) {
    debug_printf("Loading quests - find all quests in quest directory\n");
    
    if(!world) {
        error_printf("Error: world NULL.\n");
        return RESULT_ERROR;
    }

    if(!player) {
        error_printf("Error: player NULL.\n");
        return RESULT_ERROR;
    }

    /* Don't know how many quests files are stored - try until not found */
    char quests_data_path[LOADER_RESOURCE_PATH_BUFFER_SIZE];
    int quest_id = 1;
    bool_t found = BOOL_FALSE;
    do {
        found = BOOL_FALSE;
        memset(quests_data_path, '\0', LOADER_RESOURCE_PATH_BUFFER_SIZE * sizeof(char));
        sprintf(quests_data_path, "%s/quest_%d.json", LOADER_RESOURCES_QUESTS_DIR, quest_id);

        if(loader_from_file(quests_data_path, file_read_buffer, CORE_FILE_READ_BUFFER_SIZE) != RESULT_OK) {
            debug_printf("Quest %d file not loaded from %s. Can finish iterating.\n", quest_id, quests_data_path);
        } 
        
        /* Quest data exist - process it*/
        else {
            found = BOOL_TRUE;
            
            /* Extract quest data */
            cJSON* quest_json = cJSON_Parse(file_read_buffer);

            if(loader_load_quest_data_from_cJSON(quest_json, world, player, quest_id) != RESULT_OK) {
                error_printf("Error: loading quest %d failed.\n", quest_id);
                cJSON_Delete(quest_json);
                return RESULT_ERROR;
            }
            cJSON_Delete(quest_json);
        }
        ++quest_id;
    } while(found == BOOL_TRUE);

    /* Loaded what was to be loaded */
    return RESULT_OK;
}




/**
 * Load single quest
*/
static result_t loader_load_quest_data_from_cJSON(const cJSON* json, world_t* world, player_t* player, const int quest_id) {
    info_printf("Processing quest %d\n", quest_id);

    /* Get quest id*/
    int loaded_quest_id;
    if(loader_get_int_from_cJSON(json, "id", &loaded_quest_id) == OPTION_NONE) {
        error_printf("Quest data missing id.\n");
        return RESULT_ERROR;
    }

    /* Check if player has this quest - id my be unique! */
    if(questlog_has_quest(&player->questlog, loaded_quest_id) == BOOL_TRUE) {
        error_printf("Quest %d already in questlog. Seems files are corrupted.\n", loaded_quest_id);
        return RESULT_ERROR;
    }

    /* Add quest to questlog with 0 stage - QUEST_STAGE_NONE */
    if(questlog_append_quest(&player->questlog, loaded_quest_id, QUEST_STAGE_NONE) == RESULT_ERROR) {
        error_printf("Appending quest %d to questlog failed.\n", loaded_quest_id);
        return RESULT_ERROR;
    }

    /* Get quest name */
    //todo

    /* Get quest description */
    //todo
    
    /* Get quest stages */
    cJSON* stages_json = cJSON_GetObjectItem(json, "quest_stages");

    if(!stages_json) {
        error_printf("Error: quest missing quest_stages.\n");
        return RESULT_ERROR;
    }

    if(!cJSON_IsArray(stages_json)) {
        error_printf("Error: stages_json is not an array.\n");
        return RESULT_ERROR;
    }

    /* Iterate over quest_stage stages */
    cJSON* stage_json = NULL;
    cJSON_ArrayForEach(stage_json, stages_json) {
        if(!cJSON_IsObject(stage_json)) {
            error_printf("Error: stage_json is not an object.\n");
            return RESULT_ERROR;
        }

        /* Get quest stage, check if unique and in order */
        int quest_stage;
        if(loader_get_int_from_cJSON(stage_json, "quest_stage", &quest_stage) == OPTION_NONE) {
            error_printf("Error: quest stage missing quest_stage.\n");
            return RESULT_ERROR;
        }

        /* Get quest objective */
        //todo

        /* NPC to talk to id */
        int npc_to_talk_id;
        if(loader_get_int_from_cJSON(stage_json, "npc_to_talk_id", &npc_to_talk_id) == OPTION_NONE) {
            error_printf("Error: quest stage missing npc_to_talk_id.\n");
            return RESULT_ERROR;
        }

        /* Get NPC pointer */
        npc_t* npc_to_talk;
        if(world_get_npc_by_id(world, &npc_to_talk, npc_to_talk_id) == OPTION_NONE) {
            error_printf("Error: npc_to_talk_id %d not found in world.\n", npc_to_talk_id);
            return RESULT_ERROR;
        }
        debug_printf("npc_to_talk_id %d id %s\n", npc_to_talk_id, npc_to_talk->name);

        /* Get quest dialog blocks */
        cJSON* stage_dialog_blocks_json = cJSON_GetObjectItem(stage_json, "dialog_blocks");

        if(!stage_dialog_blocks_json) {
            error_printf("Error: quest stage missing dialog_blocks.\n");
            return RESULT_ERROR;
        }

        if(!cJSON_IsArray(stage_dialog_blocks_json)) {
            error_printf("Error: stage_dialog_blocks_json is not an array.\n");
            return RESULT_ERROR;
        }
        
        /* Common context for all dialog_blocks */
        loader_context_t loader_context = {
            .npc_to_talk_to_id = npc_to_talk_id,
            .quest_id = loaded_quest_id,
            .quest_stage = quest_stage,
        };

        /* Iterate over stage dialog blocks */
        cJSON* stage_dialog_block_json = NULL;
        cJSON_ArrayForEach(stage_dialog_block_json, stage_dialog_blocks_json) {
            if(!cJSON_IsObject(stage_dialog_block_json)) {
                error_printf("Error: room_json is not an object.\n");
                return RESULT_ERROR;
            }

            /* Extract dialog block and give it to NPC */
            dialog_block_t dialog_block;
            if(dialog_block_init(&dialog_block, DIALOG_TYPE_QUEST) != RESULT_OK) {
                error_printf("Error: dialog block init failed.\n");
                return RESULT_ERROR;
            }
            

            if(loader_get_dialog_block_from_cJSON(stage_dialog_block_json, &dialog_block, (void *)&loader_context) != RESULT_OK) {
                error_printf("Error: loading dialog block failed.\n");
                return RESULT_ERROR;
            }

            /* Add dialog block to NPC */
            if(npc_append_dialog_block(npc_to_talk, &dialog_block) != RESULT_OK) {
                error_printf("Error: appending dialog block to NPC failed.\n");
                return RESULT_ERROR;
            }

            //#error


        }

    }

    /* Get stages count */
    int stages_count = cJSON_GetArraySize(stages_json);


    // todo - use stages_count
    UNUSED(stages_count);
    return RESULT_OK;
}