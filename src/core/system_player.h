#pragma once
#include "utils.h"
#include "player.h"
#include "world.h"
#include "item_database.h"

result_t system_player_change_room(world_t* world, player_t* player, int room_id);
option_t system_player_get_npc_in_conversation(world_t* world, player_t* player, npc_t** npc_data);


void system_player_print_player(player_t* player, item_database_t* item_database);