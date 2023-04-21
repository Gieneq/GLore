#pragma once
#include "utils.h"

typedef size_t quest_id_t;

typedef struct questlog_t {
    quest_id_t quest_id;
    int stage;
} questlog_t;

void questlog_create(questlog_t *questlog, quest_id_t quest_id);
//todo questlog get finished quests: need to share data from database

typedef struct quest_data_t {

} quest_data_t;