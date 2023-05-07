#pragma once
#include "player.h"
#include "utils.h"

typedef struct core_t core_t;

void system_player_communication_init();
void system_player_communication_process(core_t* core, player_t* player, const char* msg);
// option_t system_player_communication_should_stop(player_t* player, const char* msg);
// option_t system_player_communication_want_help(player_t* player, const char* msg);
// void system_player_communication_passed_unknown();