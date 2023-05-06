#include "npc.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//todo add registry to validate passing id's - they can be messed in data files.

result_t npc_init(npc_t* npc) {
    printf("NPC initializing\n");

    if(npc_clear(npc) != RESULT_OK) {
        printf("Cannot clear NPC data.\n");
        return RESULT_ERROR;
    }
    
    /* Dialogs */
    // todo

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
    return RESULT_OK;
}

bool_t npc_is_valid(npc_t* npc) {
    return npc->id != 0 ? TRUE : FALSE;
}

result_t npc_set_name(npc_t* npc, const char* name) {
    if(strlen(name) > NPC_NAME_MAX_LENGTH) {
        printf("New NPC name too long.\n");
        return RESULT_ERROR;
    }

    strcpy(npc->name, name);
    return RESULT_OK;
}