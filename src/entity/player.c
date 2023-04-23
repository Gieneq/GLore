#include "player.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/* player */
void player_set_name(player_t* player, const char * player_name) {
    // size_t chars_count = MIN(strlen(player_name), PLAYER_NAME_MAX_LENGTH);
    // strncpy(player->name, player_name, chars_count);
    cpystr_trimed(player->name, player_name, PLAYER_NAME_BUFFER_SIZE);
}

void player_set_stats(player_t* player, stats_t* player_stats) {
    memcpy(&(player->stats), player_stats, sizeof(stats_t));
}

bool_t player_is_alive(player_t* player) {
    return player->stats.health > 0;
}

/* Player's questlog */
size_t player_questlog_get_all_quests_count(player_t* player) {
    return player->quests.count;
}

option_t player_questlog_get_quest_by_id(player_t* player, quest_id_t quest_id, questlog_t* questlog) {
    for (size_t i = 0; i < player->quests.count; i++) {
        if (player->quests.logs[i].quest_id == quest_id) {
            questlog->quest_id = player->quests.logs[i].quest_id;
            questlog->stage = player->quests.logs[i].stage;
            return OPTION_SOME;
        }
    }
    return OPTION_NONE;
}