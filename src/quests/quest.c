#include "quest.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "utils.h"
#include "database.h"

static database_t* database = NULL;

/* Questlog */
void questlog_create(questlog_t *questlog, quest_id_t quest_id) {
    questlog->quest_id = quest_id;
    questlog->stage = 0;
}

/* Quest Requirements */

void quest_requirements_create(quest_requirements_t *requirements) {
    requirements->count = 0;
}

result_t quest_requirements_add_item(quest_requirements_t *requirements, int item_id, size_t count) {
    if(requirements == NULL) {
        return RESULT_ERROR;
    }

    if(requirements->count >= QUEST_REQUIREMENTS_ITEMS_MAX_COUNT) {
        return RESULT_ERROR;
    }

    requirements->items[requirements->count].item_id = item_id;
    requirements->items[requirements->count].count = count;
    requirements->count++;
    return RESULT_OK;
}

/* Quest Reward */
void quest_reward_create(quest_reward_t *reward) {
    reward->count = 0;
    reward->exp = 0;
}

void quest_reward_set_exp(quest_reward_t *reward, int exp) {
    reward->exp = exp;
}

result_t quest_reward_add_item(quest_reward_t *reward, int item_id, size_t count) {
    if(reward == NULL) {
        return RESULT_ERROR;
    }

    if(reward->count >= QUEST_REWARDS_ITEMS_MAX_COUNT) {
        return RESULT_ERROR;
    }

    reward->items[reward->count].item_id = item_id;
    reward->items[reward->count].count = count;
    reward->count++;
    return RESULT_OK;
}

/* Quest Dialog Data */
void quest_dialog_data_create(quest_dialog_data_t *dialog_data) {
    
}

/* Quest Stage Data */

void quest_stage_data_create(quest_stage_data_t *stage_data) {
    stage_data->stage_no = 0;
    memset(stage_data->objective, '\0', QUEST_BRIEF_BUFFER_SIZE);
    quest_requirements_create(&stage_data->requirements);
    quest_reward_create(&stage_data->reward);
}



/* Quest Data */
result_t quest_data_create(quest_data_t *quest_data, quest_id_t quest_id, const char *name, const char *brief) {
    if(quest_data == NULL) {
        return RESULT_ERROR;
    }
    
    quest_data->quest_id = quest_id;

    if(cpystr_trimed(quest_data->name, name, QUEST_NAME_BUFFER_SIZE) != NO_OVERFLOW) {
        return RESULT_ERROR;
    }

    if(cpystr_trimed(quest_data->brief, brief, QUEST_BRIEF_BUFFER_SIZE) != NO_OVERFLOW) {
        return RESULT_ERROR;
    }

    memset(quest_data->stages, 0, sizeof(quest_stage_data_t) * QUEST_STAGES_MAX_COUNT);
    quest_data->stages_count = 0;

    memset(quest_data->dialogs, 0, sizeof(quest_dialog_data_t) * QUEST_DIALOGS_MAX_COUNT);
    quest_data->dialogs_count = 0;

    // printf("Creating quest %d: %s [%llu stages] (%s)\n", (int)(quest_data->quest_id), quest_data->name, stages, quest_data->brief);
    return RESULT_OK;
}

result_t quest_data_add_stage(quest_data_t *quest_data, quest_stage_data_t *stage_data) {
    if(quest_data == NULL || stage_data == NULL) {
        return RESULT_ERROR;
    }

    if(quest_data->stages_count >= QUEST_STAGES_MAX_COUNT) {
        return RESULT_ERROR;
    }

    memcpy(&quest_data->stages[quest_data->stages_count], stage_data, sizeof(quest_stage_data_t));
    quest_data->stages_count++;
    return RESULT_OK;
}

result_t quest_data_add_dialog(quest_data_t *quest_data, quest_dialog_data_t *dialog_data) {
    if(quest_data == NULL || dialog_data == NULL) {
        return RESULT_ERROR;
    }

    if(quest_data->dialogs_count >= QUEST_DIALOGS_MAX_COUNT) {
        return RESULT_ERROR;
    }

    memcpy(&quest_data->dialogs[quest_data->dialogs_count], dialog_data, sizeof(quest_dialog_data_t));
    quest_data->dialogs_count++;
    return RESULT_OK;
}




/* Printf functions */

void quest_requirements_printf(quest_requirements_t *requirements) {
    printf(" Requirements: %llu items:\n", requirements->count);
    for(size_t i = 0; i < requirements->count; i++) {
        item_data_t item_data;
        if(database_get_item_data_by_id(database, requirements->items[i].item_id, &item_data) == RESULT_OK) {
        }
        printf("  - Item %d (%s) x %llu\n", requirements->items[i].item_id, item_data.name, requirements->items[i].count);
    }
}

void quest_reward_printf(quest_reward_t *reward) {
    printf(" Reward: %d exp, %llu items:\n", reward->exp, reward->count);
    for(size_t i = 0; i < reward->count; i++) {
        item_data_t item_data;
        if(database_get_item_data_by_id(database, reward->items[i].item_id, &item_data) == RESULT_OK) {
        }
        printf("  - Item %d (%s) x %llu\n", reward->items[i].item_id, item_data.name, reward->items[i].count);
    }
}

void quest_stage_data_printf(quest_stage_data_t *stage_data) {
    printf(" Stage %llu: %s\n", stage_data->stage_no, stage_data->objective);
    quest_requirements_printf(&stage_data->requirements);
    const char *npc_name = "Unknown todo";
    quest_reward_printf(&stage_data->reward);
}

void quest_data_printf(quest_data_t *quest_data) {
    printf("Summary of quest %d: %s [%d stages] (%s)\n", (int)(quest_data->quest_id), quest_data->name, quest_data->stages_count, quest_data->brief);
    printf("Stages:\n");
    for(size_t i = 0; i < quest_data->stages_count; i++) {
        quest_stage_data_printf(&quest_data->stages[i]);
    }
    printf("Dialogs:\n");
}

result_t quest_data_attach_db_for_printf(void* db) {
    if(db == NULL) {
        return RESULT_ERROR;
    }

    if(database != NULL) {
        return RESULT_ERROR;
    }

    database = (database_t*)db;
        return RESULT_OK;
}


