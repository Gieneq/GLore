#include "dialog.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * DIALOG CONDITION IF
 */

result_t dialog_cond_if_init(dialog_cond_if_t *cond_if) {
    if(!cond_if) {
        error_printf("Dialog condition IF corrupted.\n");
        return RESULT_ERROR;
    }

    cond_if->dialog_stage = DIALOG_STAGE_NONE;
    keywords_list_init(&cond_if->keywords);
    keyword_wildcard_init(&cond_if->keyword_wildcard, WILDCARD_TYPE_NONE);
    return RESULT_OK;
}


bool_t dialog_cond_then_has_response(const dialog_cond_then_t *cond_then) {
    if(!cond_then) {
        return BOOL_FALSE;
    }
    if(strlen(cond_then->response.text) <= 0) {
        return BOOL_FALSE;
    }
    return BOOL_TRUE;
}


/*
 * DIALOG CONDITION THEN
 */

result_t dialog_cond_then_init(dialog_cond_then_t *cond_then) {
    if(!cond_then) {
        printf("Dialog condition THEN corrupted.\n");
        return RESULT_ERROR;
    }

    response_init(&cond_then->response);
    cond_then->next_dialog_stage = DIALOG_STAGE_NONE;

    return RESULT_OK;
}

/*
 * DIALOG BLOCK
 */

result_t dialog_block_init(dialog_block_t *block, const dialog_type_t dial_type) {
    debug_printf("Dialog block initializing\n");

    if(dialog_block_clear(block) != RESULT_OK) {
        error_printf("Cannot clear dialog block.\n");
        return RESULT_ERROR;
    }

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

    dialog_block->type = DIALOG_TYPE_INVALID;
    /* Used to make dialog block invalid */
    return RESULT_OK;
}

bool_t dialog_block_is_valid(dialog_block_t *dialog_block) {
    return dialog_block->type != DIALOG_TYPE_INVALID ? BOOL_TRUE : BOOL_FALSE;
}

void dialog_cond_if_printf(const dialog_cond_if_t *cond_if) {
#if DEBUG == 1
    printf("#DEBUG ");
#endif
    printf("Cond if stage: %d, with keywords:", cond_if->dialog_stage);
    for(int i=0; i<cond_if->keywords.count; ++i) {
        printf("\'%s\' ", cond_if->keywords.keywords[i].text);
    }
    printf(" wildcard: %d(%d)\n", cond_if->keyword_wildcard.type, cond_if->keyword_wildcard.id);
}

void dialog_cond_then_printf(const dialog_cond_then_t *cond_then) {
#if DEBUG == 1
    printf("#DEBUG ");
#endif
    printf("Cond then enxt stage: %d response", cond_then->next_dialog_stage);
    if(dialog_cond_then_has_response(cond_then) == BOOL_TRUE) {
        printf(": %s\n", cond_then->response.text);
    } else {
        printf(" empty\n");
    }
}

void dialog_block_printf(const dialog_block_t *cond) {
    dialog_cond_if_printf(&cond->cond_if);
    dialog_cond_then_printf(&cond->cond_then);
}