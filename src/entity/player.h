#pragma once
#include "utils.h"
#include "quest.h"
#include "room.h"
#include "container.h"

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
    int current_room_id;
    int current_conversation_npc_id;
    questlog_t questlog;
    container_t backpack;
} player_t;

void player_init(player_t* player);

void player_set_name(player_t* player, const char * player_name);
void player_set_stats(player_t* player, stats_t* player_stats);

bool_t player_is_alive(player_t* player);

/* Conversation */
bool_t player_is_in_conversation(const player_t* player);
void player_leave_conversation(player_t* player);