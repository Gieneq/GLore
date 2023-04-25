#include "questlog.h"


void questlog_init(questlog_t *questlog) {
    questlog->quest_id = 0;
    questlog->stage = 0;
}

/* Questlog */
void questlog_create(questlog_t *questlog, quest_id_t quest_id) {
    questlog_init(questlog);
    questlog->stage = 0;
}
