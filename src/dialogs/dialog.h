#pragma once
#include "utils.h"
#include "keywords.h"
#include "responses.h"
#include "questlog.h"

typedef enum dialog_type_t {
    DIALOG_TYPE_REGULAR = 0xFF,
    DIALOG_TYPE_QUEST = 0xFF00,
    DIALOG_TYPE_SHOP = 0xFF0000,
} dialog_type_t;

typedef struct dialog_cond_if_t {
    int dialog_stage;
    keywords_list_t keywords;
    questlog_t questlog;
    int entering_quest_id;
    bool_t check_requirements;
} dialog_cond_if_t;

void dialog_cond_if_init(dialog_cond_if_t *cond_if);


typedef struct dialog_cond_then_t {

} dialog_cond_then_t;

void dialog_cond_then_init(dialog_cond_then_t *cond_then);

typedef struct dialog_cond_t {
    dialog_type_t type;
    dialog_cond_if_t cond_if;
    dialog_cond_then_t cond_then;
} dialog_cond_t;

void dialog_cond_init(dialog_cond_t *cond, const dialog_type_t dial_type);
void dialog_cond_printf(const dialog_cond_t *cond);