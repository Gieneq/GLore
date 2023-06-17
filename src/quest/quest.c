#include "quest.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "utils.h"


void quest_stage_init(quest_stage_t* quest_stage_data, int quest_id, int stage) {
    quest_stage_data->quest_id = quest_id;
    quest_stage_data->stage = stage;
}


/* Questlog */

void questlog_init(questlog_t* questlog) {
    memset(questlog->quests, 0, sizeof(quest_stage_t) * PLAYER_QUESTLOG_MAX_COUNT);
    for(int i = 0; i < PLAYER_QUESTLOG_MAX_COUNT; i++) {
        quest_stage_init(&questlog->quests[i], INVALID_ID, QUEST_STAGE_NONE);
    }
    questlog->size = 0;
}


bool_t questlog_has_quest(questlog_t* questlog, int quest_id) {
    for(int i = 0; i < questlog->size; i++) {
        if(questlog->quests[i].quest_id == quest_id) {
            return BOOL_TRUE;
        }
    }
    return BOOL_FALSE;
}


result_t questlog_append_quest(questlog_t* questlog, int quest_id, int stage) {
    if(questlog->size >= PLAYER_QUESTLOG_MAX_COUNT) {
        return RESULT_ERROR;
    }
    quest_stage_init(&questlog->quests[questlog->size], quest_id, stage);
    questlog->size++;
    return RESULT_OK;
}

option_t questlog_get_quest_stage(questlog_t* questlog, int quest_id, int* stage) {
    for(int i = 0; i < questlog->size; i++) {
        if(questlog->quests[i].quest_id == quest_id) {
            *stage = questlog->quests[i].stage;
            return OPTION_SOME;
        }
    }
    return OPTION_NONE;
}

option_t questlog_get_quest_by_id(questlog_t* questlog, int quest_id, quest_stage_t** quest_stage) {
    for(int i = 0; i < questlog->size; i++) {
        if(questlog->quests[i].quest_id == quest_id) {
            *quest_stage = &questlog->quests[i];
            return OPTION_SOME;
        }
    }
    return OPTION_NONE;
}

option_t questlog_update(questlog_t* questlog, int quest_id, int stage) {
    /* If player has quest in questlog */
    if(questlog_has_quest(questlog, quest_id) == BOOL_TRUE) {
        quest_stage_t* quest_stage;
        if(questlog_get_quest_by_id(questlog, quest_id, &quest_stage) == OPTION_NONE) {
            /* Should not happen */
            error_printf("Updating quest %d/%d failed!\n", quest_id, stage);
            return OPTION_NONE;
        }
        quest_stage->stage = stage;
        return OPTION_SOME;
    }

    /* Else player doesn't have quest in questlog */
    if(questlog_append_quest(questlog, quest_id, stage) != RESULT_OK) {
        error_printf("Appending quest %d/%d failed!\n", quest_id, stage);
        return OPTION_NONE;
    }

    return OPTION_SOME;
}

void questlog_print(questlog_t* questlog) {
    if(questlog->size == 0) {
        info_printf("Questlog empty\n");
    }
    for(int i=0; i<questlog->size; ++i) {
        quest_stage_t* stage = &questlog->quests[i];
        if(stage->stage == QUEST_STAGE_NONE) {
            info_printf("%d: %s,\n", stage->quest_id, "Not picked yet");
        } else {
            info_printf("%d: %d,\n", stage->quest_id, stage->stage);
        }
    }
}