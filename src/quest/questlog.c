#include "questlog.h"


void quest_progress_init(quest_progress_t *quest_progress) {
    quest_progress->quest_id = 0;
    quest_progress->stage = 0;
}

/* Questlog */
void quest_progress_create(quest_progress_t *quest_progress, quest_id_t quest_id) {
    quest_progress_init(quest_progress);
    quest_progress->stage = 0;
}
