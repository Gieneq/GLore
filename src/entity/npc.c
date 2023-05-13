#include "npc.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "player.h"

//todo add registry to validate passing id's - they can be messed in data files.

result_t npc_init(npc_t* npc) {
    debug_printf("NPC initializing with some basic hi-bye dialog\n");

    if(npc_clear(npc) != RESULT_OK) {
        error_printf("Cannot clear NPC data.\n");
        return RESULT_ERROR;
    }
    
    /* Dialogs */
    /* Add every NPC Hi/Hello name - Bye dialog transition */

    {
        /* Greeting block */
        dialog_block_t greeting_dialog_block;
        dialog_block_init(&greeting_dialog_block, DIALOG_TYPE_REGULAR);

        dialog_cond_if_t* cond_if = &greeting_dialog_block.cond_if;
        
        /* From (if) stage 0 */
        cond_if->dialog_stage = 0;

        /* With (if) keywords */
        if(keywords_list_from_delimited_string(&cond_if->keywords, "hi, hello, greetings", ",") != RESULT_OK) {
            error_printf("Couldnt parse keywords.\n");
            return RESULT_ERROR;
        }

        cond_if->keyword_wildcard.type = WILDCARD_TYPE_NPC_NAME; //todo as function

        // printf("Keywords greeting count %d.\n", greeting_dialog_block.cond_if.keywords.count);
        // if(keywords_list_match(&greeting_dialog_block.cond_if.keywords, "hi jghfg") == OPTION_SOME) {
        //     printf("Matching greeting keywords OK.\n");
        // }

        // if(dialog_block_is_valid(&greeting_dialog_block) == FALSE) {
        //     printf("Cannot initialize NPC greeting dialog block.\n");
        //     return RESULT_ERROR;
        // }

        /* To (then) stage 1 */
        greeting_dialog_block.cond_then.next_dialog_stage = 1;

        response_from_string(&greeting_dialog_block.cond_then.response, "Yo nyga", FALSE);

        /* Append greeting block */
        if(npc_append_dialog_block(npc, &greeting_dialog_block) != RESULT_OK) {
            /* BTW appending leaves variable invalid */
            error_printf("Cannot add dialog block to NPC.\n");
            return RESULT_ERROR;
        }
    }
    
    /* Goodbye block */
    {
        dialog_block_t goodbye_dialog_block;
        dialog_block_init(&goodbye_dialog_block, DIALOG_TYPE_REGULAR);

        /* From (if) stage 1 */
        goodbye_dialog_block.cond_if.dialog_stage = 1;

        /* With (if) keywords */
        if(keywords_list_from_delimited_string(&goodbye_dialog_block.cond_if.keywords, "bye, cu, goodbye, farewell", ",") != RESULT_OK) {
            error_printf("Couldnt parse keywords.\n");
            return RESULT_ERROR;
        }

        goodbye_dialog_block.cond_if.keyword_wildcard.type = WILDCARD_TYPE_NPC_NAME; //todo as function
        
        /* To (then) stage 1 */
        goodbye_dialog_block.cond_then.next_dialog_stage = 0;
        

        response_from_string(&goodbye_dialog_block.cond_then.response, "Bye bye", FALSE);

        /* Append greeting block */
        if(npc_append_dialog_block(npc, &goodbye_dialog_block) != RESULT_OK) {
            /* BTW appending leaves variable invalid */
            printf("Cannot add dialog block to NPC.\n");
            return RESULT_ERROR;
        }

        npc_set_last_block_as_drop_conversation(npc);
    }

    debug_printf("NPC initialized with %d dialog blocks. Has drop? %c\n", npc->dialog_blocks_count, npc_get_drop_cond_then(npc) ? 'Y' : 'N');

    return RESULT_OK;
}

result_t npc_clear(npc_t* npc) {
    if(!npc) {
        error_printf("NPC data corrupted.\n");
        return RESULT_ERROR;
    }

    /* ID */
    npc->id = 0;

    /* Name */
    memset(npc->name, '\0', NPC_NAME_BUFFER_SIZE);

    /* Dialogs */
    npc->dialog_stage = 0;
    memset(npc->dialog_blocks, 0, sizeof(dialog_block_t) * NPC_DIALOG_BLOCKS_MAX_COUNT);
    npc->dialog_blocks_count = 0;

    /* Drop dialog (then) block */
    npc->dialog_drop_cond_then_index = DIALOG_COND_THEN_NO_DROP;

    return RESULT_OK;
}

bool_t npc_is_valid(npc_t* npc) {
    return npc->id != 0 && strlen(npc->name) > 0 ? TRUE : FALSE;
}

result_t npc_set_name(npc_t* npc, const char* name) {
    if(strlen(name) > NPC_NAME_MAX_LENGTH) {
        error_printf("New NPC name too long.\n");
        return RESULT_ERROR;
    }

    strcpy(npc->name, name);
    return RESULT_OK;
}

result_t npc_append_dialog_block(npc_t* npc, dialog_block_t* dialog_block) {
    if(npc->dialog_blocks_count >= NPC_DIALOG_BLOCKS_MAX_COUNT) {
        error_printf("Failed to add dialog block to NPC: NPC is full\n");
        return RESULT_ERROR;
    }

    if(!dialog_block) {
        error_printf("Dialog block data corrupted.\n");
        return RESULT_ERROR;
    }

    if(dialog_block_is_valid(dialog_block) == FALSE) {
        error_printf("Dialog block data is invalid.\n");
        return RESULT_ERROR;
    }

    /* Copy dialog block data */
    int new_dialog_block_index = npc->dialog_blocks_count;
    memcpy(&npc->dialog_blocks[new_dialog_block_index], dialog_block, sizeof(dialog_block_t));

    /* After moving leave dialog block data invalid */
    if(dialog_block_clear(dialog_block) != RESULT_OK) {
        error_printf("Cannot clear dialog block data after moving.\n");
        // clear occupied memory?
        return RESULT_ERROR;
    }

    ++(npc->dialog_blocks_count);
    return RESULT_OK;
}


option_t npc_match_user_input(npc_t* npc, player_t* player, const char* msg) {
    if(!npc || !msg) {
        error_printf("NPC data corrupted.\n");
        return OPTION_NONE;
    }

    if(npc_is_valid(npc) == FALSE) {
        error_printf("NPC data is invalid.\n");
        return OPTION_NONE;
    }

    debug_printf("NPC %s with %d dialog blocks:\n", npc->name, npc->dialog_blocks_count);
    /* Iterate through all dialog blocks */
    for(int i=0; i<npc->dialog_blocks_count; ++i) {
        dialog_block_t* dialog_block = &npc->dialog_blocks[i];

        /* Check (if) block */
        if(dialog_cond_if_match(&dialog_block->cond_if, npc, player, msg) == OPTION_SOME) {
            debug_printf("Matched with block:\n");
#if DEBUG == 1
        dialog_block_printf(dialog_block);
#endif

            /* Execute (then) block */
            if(dialog_cond_then_execute(&dialog_block->cond_then, npc, player) != RESULT_OK) {
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


result_t npc_set_last_block_as_drop_conversation(npc_t* npc) {
    if(!npc) {
        error_printf("NPC data corrupted.\n");
        return RESULT_ERROR;
    }

    if(npc->dialog_blocks_count == 0) {
        error_printf("NPC has no dialog blocks.\n");
        return RESULT_ERROR;
    }

    npc->dialog_drop_cond_then_index = npc->dialog_blocks_count - 1;
    debug_printf("NPC %s set last block as drop conversation %d.\n", npc->name, npc->dialog_blocks_count - 1);
    return RESULT_OK;
}

dialog_cond_then_t* npc_get_drop_cond_then(npc_t* npc) {
    if(!npc) {
        error_printf("NPC data corrupted.\n");
        return NULL;
    }

    if(npc->dialog_drop_cond_then_index < 0) {
        error_printf("NPC has no drop dialog block.\n");
        return NULL;
    }

    return &npc->dialog_blocks[npc->dialog_drop_cond_then_index].cond_then;
}

bool_t npc_is_in_conversation(npc_t* npc) {
    return npc->dialog_stage != 0 ? TRUE : FALSE;
}

void npc_leave_conversation(npc_t* npc) {
    dialog_cond_then_t* dialog_cond_then_drop = npc_get_drop_cond_then(npc);
    if(dialog_cond_then_drop) {
        if(dialog_cond_then_execute(dialog_cond_then_drop, npc, NULL) != RESULT_OK) {
            /* Should not happen, crash somehow */
            error_printf("Cannot execute dialog then case.\n");
            HARDFAULT();
        }
    } else {
        error_printf("Missing drop condition.\n");
    }
}