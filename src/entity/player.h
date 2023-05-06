#pragma once
#include "utils.h"
#include "quest.h"
// #include 

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
        questlog_t logs[PLAYER_QUESTLOG_BUFFER_SIZE];
        size_t count;
    } quests;
    room_t* current_room;
} player_t;

void player_set_name(player_t* player, const char * player_name);
void player_set_stats(player_t* player, stats_t* player_stats);

bool_t player_is_alive(player_t* player);
size_t player_questlog_get_all_quests_count(player_t* player);
option_t player_questlog_get_quest_by_id(player_t* player, quest_id_t quest_id, questlog_t* questlog);