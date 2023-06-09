#include "system_dialogs.h"
#include "system_quest.h"

option_t system_dialog_match_dialog_stage(const dialog_cond_if_t *cond_if, npc_t* npc) {
    return cond_if->dialog_stage == npc->dialog_stage ? OPTION_SOME : OPTION_NONE;
}

option_t system_dialog_match_any_npc_name(const char* npc_name, const char* msg) {
    keyword_t kw_npc_name;
    keyword_from_string(&kw_npc_name, npc_name);
    if(keyword_match_any_ignorecase(&kw_npc_name, msg) != OPTION_SOME) {
        return OPTION_NONE;
    }
    return OPTION_SOME;
}

//todo - probably need to extract logic to check everything against a keyword.
option_t system_dialog_match_cond_if(const dialog_cond_if_t *cond_if, npc_t* npc, player_t* player, const char *msg) {
    /* Check dialog stage */
    if(system_dialog_match_dialog_stage(cond_if, npc)) {
        return OPTION_NONE;
    }

    /* Check questlog stage if related */
    bool_t is_quest_matching = BOOL_FALSE;
    // dialog_cond_if_printf(cond_if, "\n");
    if(system_quest_get_cond_if_mach_player_questlog(cond_if, player, &is_quest_matching) == OPTION_SOME) {
        /* Cond_if has some quest relate stuff */
        if(is_quest_matching == BOOL_FALSE) {
            // printf(" Q not matching!\n");
            return OPTION_NONE;
        }

        // printf(" Q-is matching!\n");
    } 
    
    /* Seems any quest conditions (if are present) are ok */

    // dialog_cond_if_printf(cond_if);
    debug_printf("   stages OK.\n");

    /* Check keywords */
#if DEBUG == 1
    dialog_cond_if_printf(cond_if, "\n");
#endif

    if(keywords_list_match_any_ignorecase(&cond_if->keywords, msg) != OPTION_SOME) {
        return OPTION_NONE;
    }
    debug_printf("   keywords OK.\n");

    /* Check wildcards */
    switch (cond_if->keyword_wildcard.type)
    {
    case WILDCARD_TYPE_NPC_NAME:
        {
            /* Create keyword from NPC name */
            if(system_dialog_match_any_npc_name(npc->name, msg) != OPTION_SOME) {
                return OPTION_NONE;
            }
        }
        break;
    case WILDCARD_TYPE_ITEM_NAME:
        {
        }
        break;
    
    default:
        break;
    }

    /* Only one conversation at the time. If stage 0 check */
    /* if player is already in any conversation */
    if(cond_if->dialog_stage == 0) {
        if(player_is_in_conversation(player) == BOOL_TRUE) {
            info_printf("You are already in conversation with.\n");
            return OPTION_NONE;
        } else {
            player->current_conversation_npc_id = npc->id;
        }
    }

    /* Check quest */

    return OPTION_SOME;
}


result_t system_dialog_execute_condition_then(const dialog_cond_then_t *cond_then, npc_t* npc, player_t* player) {
    /* Change NPC dialog stage */
    npc->dialog_stage = cond_then->next_dialog_stage;
    debug_printf("NPC %s dialog stage changed to %d.\n", npc->name, npc->dialog_stage);

    /* Manipulate players inventory */

    /* Manipulate questlog */
    bool_t was_quest_fullfilled;
    if(system_quest_execute_cond_then_match_player_questlog(cond_then, player, &was_quest_fullfilled) == OPTION_SOME) {
        /* Seems there are quest related effects */
        if(was_quest_fullfilled == BOOL_FALSE) {
            return RESULT_ERROR;
        }
    }

    /* Seems any quest related effects (if present) are executed */

    /* Say response */
#if DEBUG == 1
    dialog_cond_then_printf(cond_then, "\n");
#endif
    if(dialog_cond_then_has_response(cond_then) == BOOL_TRUE){
        response_printf("%s says: \"%s\"\n", npc->name, cond_then->response.text); //todo build
    }

    /* Leave conversation if any */
    if((cond_then->next_dialog_stage == INVALID_ID) && (player_is_in_conversation(player) == BOOL_TRUE)) {
        player_leave_conversation(player);
    }

    return RESULT_OK;
}







option_t system_dialog_match_user_input(npc_t* npc, player_t* player, const char* msg) {
    if(!npc || !msg) {
        error_printf("NPC data corrupted.\n");
        return OPTION_NONE;
    }

    if(npc_is_valid(npc) == BOOL_FALSE) {
        error_printf("NPC data is invalid.\n");
        return OPTION_NONE;
    }

    // debug_printf("NPC %s with %d dialog blocks:\n", npc->name, npc->dialog_blocks_count);
    /* Iterate through all dialog blocks */
    for(int i=0; i<npc->dialog_blocks_count; ++i) {
        dialog_block_t* dialog_block = &npc->dialog_blocks[i];

        /* Check (if) block */
        if(system_dialog_match_cond_if(&dialog_block->cond_if, npc, player, msg) == OPTION_SOME) {
            debug_printf("Matched with block:\n");
#if DEBUG == 1
        dialog_block_printf(dialog_block);
#endif

            /* Execute (then) block */
            if(system_dialog_execute_condition_then(&dialog_block->cond_then, npc, player) != RESULT_OK) {
                /* Should not happen, crash somehow */
                error_printf("Cannot execute dialog then case.\n");
                HARDFAULT();
                // todo - consider it, if NPC want to give player an item and inventory is full it will fail. Need to be safe.
            }

            // debug_printf("NPC %s entered stage %d\n", npc->name, dialog_block->cond_then.next_dialog_stage);

            return OPTION_SOME;
        }
    }


    return OPTION_NONE;
}




void system_dialog_npc_leave_conversation(npc_t* npc, player_t* player) {
    dialog_cond_then_t* dialog_cond_then_drop = npc_get_drop_cond_then(npc);
    if(dialog_cond_then_drop) {
        if(system_dialog_execute_condition_then(dialog_cond_then_drop, npc, player) != RESULT_OK) {
            /* Should not happen, crash somehow */
            error_printf("Cannot execute dialog then case.\n");
            HARDFAULT();
        }
    } else {
        error_printf("Missing drop condition.\n");
    }
}