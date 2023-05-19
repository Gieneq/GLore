#pragma once
#include "npc.h"
#include "dialog.h"
#include "player.h"

option_t system_dialog_match_cond_if(const dialog_cond_if_t *cond_if, npc_t* npc, player_t* player, const char *msg);
result_t system_dialog_execute_condition_then(const dialog_cond_then_t *cond_then, npc_t* npc, player_t* player);
option_t system_dialog_match_user_input(npc_t* npc, player_t* player, const char* msg);
void system_dialog_npc_leave_conversation(npc_t* npc);