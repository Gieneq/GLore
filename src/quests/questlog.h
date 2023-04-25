#pragma once 

// typedef struct questlog_t {
//     int quest_id;
//     int quest_stage;
// } questlog_t;


/* Quest ID */
typedef size_t quest_id_t;

/* Questlog */
typedef struct questlog_t {
    quest_id_t quest_id;
    int stage;
} questlog_t;

void questlog_create(questlog_t *questlog, quest_id_t quest_id);
void questlog_init(questlog_t *questlog);