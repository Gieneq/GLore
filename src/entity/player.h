#pragma once
#include "utils.h"
#include "quest.h"
#include "room.h"

typedef struct room_t room_t;


#define PLAYER_NAME_BUFFER_SIZE 64
#define PLAYER_NAME_MAX_LENGTH (PLAYER_NAME_BUFFER_SIZE - 1)

#define PLAYER_QUESTLOG_BUFFER_SIZE 128

typedef struct stats_t{
    int experience;
    int health;
} stats_t;

typedef struct player_t {
    char name[PLAYER_NAME_BUFFER_SIZE];
    stats_t stats;
    struct {
        quest_progress_t logs[PLAYER_QUESTLOG_BUFFER_SIZE];
        size_t count;
    } quests;
    int current_room_id; //replaced pointer
    int current_conversation_npc_id;
} player_t;

void player_set_name(player_t* player, const char * player_name);
void player_set_stats(player_t* player, stats_t* player_stats);

bool_t player_is_alive(player_t* player);
size_t player_questlog_get_all_quests_count(player_t* player);
option_t player_questlog_get_quest_by_id(player_t* player, quest_id_t quest_id, quest_progress_t* questlog);
bool_t player_is_in_conversation(const player_t* player);
void player_leave_conversation(player_t* player);