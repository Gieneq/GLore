#pragma once 
#include <stdlib.h>
// typedef struct questlog_t {
//     int quest_id;
//     int quest_stage;
// } questlog_t;


/* Quest ID */
typedef size_t quest_id_t;

/* Questlog */
typedef struct quest_progress_t {
    quest_id_t quest_id;
    int stage;
} quest_progress_t;

void quest_progress_create(quest_progress_t *quest_progress, quest_id_t quest_id);
void quest_progress_init(quest_progress_t *quest_progress);