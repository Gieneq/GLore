#pragma once
#include "utils.h"
#include "keywords.h"
#include "responses.h"
#include "questlog.h"

typedef struct npc_t npc_t;
typedef struct player_t player_t;

/* Dialog type */
typedef enum dialog_type_t {
    DIALOG_TYPE_INVALID = 0x00,
    DIALOG_TYPE_REGULAR = 0xFF,
    DIALOG_TYPE_QUEST = 0xFF00,
    DIALOG_TYPE_SHOP = 0xFF0000,
} dialog_type_t;

#define DIALOG_STAGE_NONE -1

/* Condition IF */
typedef struct dialog_cond_if_t {
    int dialog_stage;
    keywords_list_t keywords;
    keyword_wildcard_t keyword_wildcard;
    // quest_progress_t quest_progress_required; // everytime checks both id and stage
    // int entering_quest_id;
    // bool_t check_requirements;
} dialog_cond_if_t;

result_t dialog_cond_if_init(dialog_cond_if_t *cond_if);
void dialog_cond_if_printf(const dialog_cond_if_t *cond_if);
option_t dialog_cond_if_match(const dialog_cond_if_t *cond_if, npc_t* npc, player_t* player, const char *msg);

/* Condition THEN */
typedef struct dialog_cond_then_t {
    int next_dialog_stage;
    response_t response;
} dialog_cond_then_t;

result_t dialog_cond_then_init(dialog_cond_then_t *cond_then);
void dialog_cond_then_printf(const dialog_cond_then_t *cond_then);
result_t dialog_cond_then_execute(const dialog_cond_then_t *cond_then, npc_t* npc, player_t* player);
bool_t dialog_cond_then_has_response(const dialog_cond_then_t *cond_then);

/* Dialog block */
typedef struct dialog_block_t {
    dialog_type_t type; /* Mask */
    dialog_cond_if_t cond_if;
    dialog_cond_then_t cond_then;
} dialog_block_t;

result_t dialog_block_init(dialog_block_t *dialog_block, const dialog_type_t dial_type);
bool_t dialog_block_is_valid(dialog_block_t *dialog_block);
result_t dialog_block_clear(dialog_block_t *dialog_block);
void dialog_block_printf(const dialog_block_t *dialog_block);