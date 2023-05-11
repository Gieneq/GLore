#include "npc.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "debug.h"

//todo add registry to validate passing id's - they can be messed in data files.

result_t npc_init(npc_t* npc) {
    debug_printf("NPC initializing with some basic hi-bye dialog\n");

    if(npc_clear(npc) != RESULT_OK) {
        printf("Cannot clear NPC data.\n");
        return RESULT_ERROR;
    }
    
    return RESULT_OK;
    /* Dialogs */
    /* Add every NPC Hi/Hello name - Bye dialog transition */

    {
        /* Greeting block */
        dialog_block_t greeting_dialog_block;
        dialog_block_init(&greeting_dialog_block, DIALOG_TYPE_REGULAR);
        
        /* From (if) stage 0 */
        greeting_dialog_block.cond_if.dialog_stage = 0;

        /* With (if) keywords */
        if(keywords_list_from_delimited_string(&greeting_dialog_block.cond_if.keywords, "hi, hello, greetings", ",") != RESULT_OK) {
            printf("Couldnt parse keywords.\n");
            return RESULT_ERROR;
        }

        printf("Keywords greeting count %d.\n", greeting_dialog_block.cond_if.keywords.count);
        if(keywords_list_match(&greeting_dialog_block.cond_if.keywords, "hi jghfg") == OPTION_SOME) {
            printf("Matching greeting keywords OK.\n");
        }

        if(dialog_block_is_valid(&greeting_dialog_block) == FALSE) {
            printf("Cannot initialize NPC greeting dialog block.\n");
            return RESULT_ERROR;
        }

        /* To (then) stage 1 */
        greeting_dialog_block.cond_then.next_dialog_stage = 1;

        /* Append greeting block */
        if(npc_append_dialog_block(npc, &greeting_dialog_block) != RESULT_OK) {
            /* BTW appending leaves variable invalid */
            printf("Cannot add dialog block to NPC.\n");
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
            printf("Couldnt parse keywords.\n");
            return RESULT_ERROR;
        }

        printf("Keywords goodbye count %d.\n", goodbye_dialog_block.cond_if.keywords.count);
        if(keywords_list_match(&goodbye_dialog_block.cond_if.keywords, "bye noob") == OPTION_SOME) {
            printf("Matching goodbye keywords OK.\n");
        }
        
        /* To (then) stage 1 */
        goodbye_dialog_block.cond_then.next_dialog_stage = 1;

        /* Append greeting block */
        if(npc_append_dialog_block(npc, &goodbye_dialog_block) != RESULT_OK) {
            /* BTW appending leaves variable invalid */
            printf("Cannot add dialog block to NPC.\n");
            return RESULT_ERROR;
        }
    }

    return RESULT_OK;
}

result_t npc_clear(npc_t* npc) {
    if(!npc) {
        printf("NPC data corrupted.\n");
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

    return RESULT_OK;
}

bool_t npc_is_valid(npc_t* npc) {
    return npc->id != 0 && strlen(npc->name) > 0 ? TRUE : FALSE;
}

result_t npc_set_name(npc_t* npc, const char* name) {
    if(strlen(name) > NPC_NAME_MAX_LENGTH) {
        printf("New NPC name too long.\n");
        return RESULT_ERROR;
    }

    strcpy(npc->name, name);
    return RESULT_OK;
}

result_t npc_append_dialog_block(npc_t* npc, dialog_block_t* dialog_block) {
    if(npc->dialog_blocks_count >= NPC_DIALOG_BLOCKS_MAX_COUNT) {
        printf("Failed to add dialog block to NPC: NPC is full\n");
        return RESULT_ERROR;
    }

    if(!dialog_block) {
        printf("Dialog block data corrupted.\n");
        return RESULT_ERROR;
    }

    if(dialog_block_is_valid(dialog_block) == FALSE) {
        printf("Dialog block data is invalid.\n");
        return RESULT_ERROR;
    }

    /* Copy dialog block data */
    int new_dialog_block_index = npc->dialog_blocks_count;
    memcpy(&npc->dialog_blocks[new_dialog_block_index], dialog_block, sizeof(dialog_block_t));

    /* After moving leave dialog block data invalid */
    if(dialog_block_clear(dialog_block) != RESULT_OK) {
        printf("Cannot clear dialog block data after moving.\n");
        // clear occupied memory?
        return RESULT_ERROR;
    }

    ++(npc->dialog_blocks_count);
    return RESULT_OK;
}


