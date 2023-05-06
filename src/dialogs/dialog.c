#include "dialog.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * DIALOG CONDITION IF
 */

result_t dialog_cond_if_init(dialog_cond_if_t *cond_if) {
    if(!cond_if) {
        printf("Dialog condition IF corrupted.\n");
        return RESULT_ERROR;
    }

    cond_if->dialog_stage = 0;
    keywords_list_init(&cond_if->keywords);
    quest_progress_init(&cond_if->quest_progress_required);
    cond_if->entering_quest_id = 0;
    cond_if->check_requirements = FALSE;
    
    return RESULT_OK;
}

/*
 * DIALOG CONDITION THEN
 */

result_t dialog_cond_then_init(dialog_cond_then_t *cond_then) {
    if(!cond_then) {
        printf("Dialog condition THEN corrupted.\n");
        return RESULT_ERROR;
    }

    //todo response

    return RESULT_OK;
}

/*
 * DIALOG BLOCK
 */

result_t dialog_block_init(dialog_block_t *block, const dialog_type_t dial_type) {
    printf("Dialog block initializing\n");

    if(dialog_block_clear(block) != RESULT_OK) {
        printf("Cannot clear dialog block.\n");
        return RESULT_ERROR;
    }

    // block->local_stage = 0;
    block->type = dial_type;

    if(dialog_cond_if_init(&block->cond_if) != RESULT_OK) {
        printf("Cannot initialize dialog condition IF.\n");
        return RESULT_ERROR;
    }

    if(dialog_cond_then_init(&block->cond_then) != RESULT_OK) {
        printf("Cannot initialize dialog condition THEN.\n");
        return RESULT_ERROR;
    }

    return RESULT_OK;
}

result_t dialog_block_clear(dialog_block_t *dialog_block) {
    if(!dialog_block) {
        printf("Dialog block corrupted.\n");
        return RESULT_ERROR;
    }

    dialog_block->type = 0;
    /* Used to make dialog block invalid */
    return RESULT_OK;
}

bool_t dialog_block_is_valid(dialog_block_t *dialog_block) {
    return dialog_block->type != 0 ? TRUE : FALSE;
}

void dialog_block_printf(const dialog_block_t *cond) {
    printf("dialog_block_t:\n");
    printf("type: %d\n", cond->type);
    printf("cond_if:\n");
    printf("dialog_stage: %d\n", cond->cond_if.dialog_stage);
    printf("keywords:\n");
    keywords_list_printf(&cond->cond_if.keywords);
    printf("questlog:\n");
    // questlog_printf(&cond->cond_if.questlog);
    // printf("entering_quest_id: %d\n", cond->cond_if.entering_quest_id);
    // printf("check_requirements: %d\n", cond->cond_if.check_requirements);
    // printf("cond_then:\n");
    // printf("dialog_stage: %d\n", cond->cond_then.dialog_stage);
    // printf("keywords:\n");
    // keywords_list_printf(&cond->cond_then.keywords);
    // printf("questlog:\n");
    // questlog_printf(&cond->cond_then.questlog);
    // printf("entering_quest_id: %d\n", cond->cond_then.entering_quest_id);
    // printf("check_requirements: %d\n", cond->cond_then.check_requirements);
}