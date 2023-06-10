#pragma once
#include "utils.h"
#include "quest.h"
#include "dialog.h"
#include "player.h"

option_t system_quest_get_cond_if_mach_player_questlog(const dialog_cond_if_t* condition_if, player_t* player, bool_t* is_maching);
option_t system_quest_execute_cond_then_match_player_questlog(const dialog_cond_then_t* condition_then, player_t* player, bool_t* was_fullfilled);