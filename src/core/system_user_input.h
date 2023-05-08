#pragma once
#include "player.h"
#include "utils.h"

typedef struct core_t core_t;

void system_user_input_init();
void system_user_input_process(core_t* core, player_t* player, const char* msg);