#pragma once
#include "utils.h"
#include "world.h"
// #include "database.h"
// #include "cJSON.h"
// #include "keywords.h"
// #include "dialog.h"

#define LOADER_INVALID_ID 0

result_t loader_load_world(world_t* world);

/*
 * Function to get world, npc, player, items and objects dagta from data files.
*/
// result_t loader_parse(world_t* world, database_t* database);
// result_t loader_keywords_list_from_json(keywords_list_t *list, const cJSON *json);
// result_t loader_dialog_cond_from_json(dialog_block_t *cond_if, const dialog_type_t dialog_type, const cJSON *json);