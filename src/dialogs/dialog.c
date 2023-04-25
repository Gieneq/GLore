#include "dialog.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


void dialog_cond_if_init(dialog_cond_if_t *cond_if) {
    cond_if->dialog_stage = 0;
    keywords_list_init(&cond_if->keywords);
    questlog_init(&cond_if->questlog);
    cond_if->entering_quest_id = 0;
    cond_if->check_requirements = FALSE;
}


void dialog_cond_then_init(dialog_cond_then_t *cond_then) {
    //TODO
}

void dialog_cond_init(dialog_cond_t *cond, const dialog_type_t dial_type) {
    cond->type = dial_type;
    dialog_cond_if_init(&cond->cond_if);
    dialog_cond_then_init(&cond->cond_then);
}

void dialog_cond_printf(const dialog_cond_t *cond) {
    printf("dialog_cond_t:\n");
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