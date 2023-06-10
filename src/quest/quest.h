#pragma once
#include "item.h"
#include "utils.h"

#define QUEST_STAGE_NONE 0
#define PLAYER_QUESTLOG_MAX_COUNT 32

/* Quest stage */
typedef struct quest_stage_t {
    int quest_id;
    int stage;
} quest_stage_t;

void quest_stage_init(quest_stage_t* quesr_stage_data, int quest_id, int stage);

/* Questlog */
typedef struct questlog_t {
    quest_stage_t quests[PLAYER_QUESTLOG_MAX_COUNT];
    int size;
} questlog_t;

void questlog_init(questlog_t* questlog);
bool_t questlog_has_quest(questlog_t* questlog, int quest_id);
result_t questlog_append_quest(questlog_t* questlog, int quest_id, int stage);
option_t questlog_get_quest_stage(questlog_t* questlog, int quest_id, int* stage);
option_t questlog_get_quest_by_id(questlog_t* questlog, int quest_id, quest_stage_t** quest_stage);
option_t questlog_update(questlog_t* questlog, int quest_id, int stage);
void questlog_print(questlog_t* questlog);