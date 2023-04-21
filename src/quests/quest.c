#include "quest.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void questlog_create(questlog_t *questlog, quest_id_t quest_id) {
    questlog->quest_id = quest_id;
    questlog->stage = 0;
}