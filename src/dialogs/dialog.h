#pragma once
#include "utils.h"
#include "keywords.h"
#include "responses.h"
#include "questlog.h"

/* Dialog type */
typedef enum dialog_type_t {
    DIALOG_TYPE_REGULAR = 0xFF,
    DIALOG_TYPE_QUEST = 0xFF00,
    DIALOG_TYPE_SHOP = 0xFF0000,
} dialog_type_t;

/* Condition IF */
typedef struct dialog_cond_if_t {
    int dialog_stage;
    keywords_list_t keywords;
    questlog_t questlog_required; // everytime checks both id and stage
    int entering_quest_id;
    bool_t check_requirements;
} dialog_cond_if_t;

void dialog_cond_if_init(dialog_cond_if_t *cond_if);


/* Condition THEN */
typedef struct dialog_cond_then_t {
    int next_dialog_stage;
} dialog_cond_then_t;

void dialog_cond_then_init(dialog_cond_then_t *cond_then);


/* Dialog block */
typedef struct dialog_block_t {
    dialog_type_t type;
    dialog_cond_if_t cond_if;
    dialog_cond_then_t cond_then;
} dialog_block_t;

void dialog_block_init(dialog_block_t *dialog_block, const dialog_type_t dial_type);
void dialog_block_printf(const dialog_block_t *dialog_block);