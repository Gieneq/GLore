#pragma once
#include "utils.h"
#include "world.h"
#include "player.h"
#include "system_player.h"
#include "item_database.h"

result_t loader_load_items_database(item_database_t* item_database);
result_t loader_load_world(world_t* world);
result_t loader_load_player(world_t* world, item_database_t* item_database, player_t* player);
result_t loader_load_quests(world_t* world, player_t* player);