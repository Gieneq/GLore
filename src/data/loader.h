#pragma once
#include "utils.h"
#include "world.h"
#include "player.h"
#include "system_player.h"

result_t loader_load_world(world_t* world);
result_t loader_load_player(world_t* world, player_t* player);
result_t loader_load_npc_with_id(world_t* world, npc_t* npc_data, int npc_id);