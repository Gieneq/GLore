#pragma once
#include "utils.h"
#include "keywords.h"
#include "responses.h"
#include "questlog.h"

/* Dialog type */
typedef enum dialog_type_t
{
    /* Type = 0 is invalid */
    DIALOG_TYPE_REGULAR = 0xFF,
    DIALOG_TYPE_QUEST = 0xFF00,
    DIALOG_TYPE_SHOP = 0xFF0000,
} dialog_type_t;

/* Condition IF */
typedef struct dialog_cond_if_t
{
    int dialog_stage;
    keywords_list_t keywords;
    quest_progress_t quest_progress_required; // everytime checks both id and stage
    int entering_quest_id;
    bool_t check_requirements;
} dialog_cond_if_t;

result_t dialog_cond_if_init(dialog_cond_if_t *cond_if);

/* Condition THEN */
typedef struct dialog_cond_then_t
{
    int next_dialog_stage;
    // response
} dialog_cond_then_t;

result_t dialog_cond_then_init(dialog_cond_then_t *cond_then);

/* Dialog block */
typedef struct dialog_block_t
{
    // int local_stage;      /* Diaglog 1,2,3; Quest 1,2,3 translates to global pool using masks */
    // redundant - dialog_stage is in cond_if
    dialog_type_t type; /* Mask */
    dialog_cond_if_t cond_if;
    dialog_cond_then_t cond_then;
} dialog_block_t;

result_t dialog_block_init(dialog_block_t *dialog_block, const dialog_type_t dial_type);
bool_t dialog_block_is_valid(dialog_block_t *dialog_block);
result_t dialog_block_clear(dialog_block_t *dialog_block);
void dialog_block_printf(const dialog_block_t *dialog_block);