#include "player.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/* player */

void player_init(player_t* player) {
    memset(player->name, '\0', PLAYER_NAME_BUFFER_SIZE);
    memset(&player->stats, 0, sizeof(stats_t));
    player->current_room_id = INVALID_ID;
    player->current_conversation_npc_id = INVALID_ID;
    questlog_init(&player->questlog);
}

void player_set_name(player_t* player, const char * player_name) {
    cpystr_trimed(player->name, player_name, PLAYER_NAME_BUFFER_SIZE);
}

void player_set_stats(player_t* player, stats_t* player_stats) {
    memcpy(&(player->stats), player_stats, sizeof(stats_t));
}

bool_t player_is_alive(player_t* player) {
    return player->stats.health > 0;
}

/* Conversation */

bool_t player_is_in_conversation(const player_t* player) {
    return player->current_conversation_npc_id != INVALID_ID ? BOOL_TRUE : BOOL_FALSE;
}

void player_leave_conversation(player_t* player) {
    player->current_conversation_npc_id = INVALID_ID;
}