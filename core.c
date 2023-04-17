#include "core.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "entity.h"

#define CORE_ARG_MAX_LENGTH 128
#define CORE_NPC_MAX_COUNT 10

typedef struct core_obj {
    core_state_t state;
    char arg_buffer[CORE_ARG_MAX_LENGTH+1];
    size_t arg_buffer_index;
    player_t player;
    npc_t npcs[CORE_NPC_MAX_COUNT];
    size_t npc_count;
} core_obj_t;

static void _core_process_say(core_t core, const char* message) {
    //printf("You say: %s\n", message);
    for(size_t i = 0; i < core->npc_count && core->npcs[i]; i++) {
        response_t response = npc_response(core->player, core->npcs[i], message);
        if(response == RESPONSE_SOME) {
            break;
        }
    }
}


static void _core_process_cmd(core_t core) {
    if (strcmp(core->arg_buffer, "quit") == 0) {
        core->state = CORE_STATE_STOPPED;
    } 
    
    else if (strcmp(core->arg_buffer, "help") == 0) {
        printf("Commands:\n");
        printf("quit - quit the game\n");
        printf("help - show this help\n");
    } 
    
    else if (strcmp(core->arg_buffer, "look") == 0) {
        player_print(core->player);
    }
    
    else if (strncmp(core->arg_buffer, "say", 3) == 0) {
        const char* sub_msg = core->arg_buffer + 4;
        if(strlen(sub_msg) > 0) {
            _core_process_say(core, sub_msg);
        } else {
            printf("What?!\n");
        }
    }

    else {
        printf("Unknown command: %s\n", core->arg_buffer);
    }

    memset(core->arg_buffer, '\0', CORE_ARG_MAX_LENGTH+1);
    core->arg_buffer_index = 0;
}


result_t core_create(core_t *core) {
    *core = malloc(sizeof(core_obj_t));
    printf("Allocationg core %llu B.\n", sizeof(core_obj_t));
    if (*core == NULL) {
        return RESULT_ERROR;
    }
    (*core)->state = CORE_STATE_RUNNING;
    memset((*core)->arg_buffer, '\0', CORE_ARG_MAX_LENGTH+1);
    (*core)->arg_buffer_index = 0;

    /* Player */
    if(player_create(&(*core)->player, "Player") != RESULT_OK) {
        return RESULT_ERROR;
    }

    /* NPCs */
    (*core)->npc_count = 0;
    for(size_t i = 0; i < CORE_NPC_MAX_COUNT; i++) {
        (*core)->npcs[i] = NULL;
    }

    npc_t npc = NULL;
    if(npc_create(&npc, "Jim Bim") != RESULT_OK) {
        return RESULT_ERROR;
    }
    (*core)->npcs[(*core)->npc_count] = npc;
    (*core)->npc_count++;

    return RESULT_OK;
}

result_t core_delete(core_t core) {
    if (core == NULL) {
        return RESULT_ERROR;
    }
    free(core);
    return RESULT_OK;
}

void core_input(core_t core) {
    char c = getchar();
    // printf("%c", c);
    if (c == '\n' || c == '\r' || c == '\0') {
        _core_process_cmd(core);
    } else {
        if (core->arg_buffer_index < CORE_ARG_MAX_LENGTH) {
            core->arg_buffer[core->arg_buffer_index] = c;
            core->arg_buffer_index++;
        }
    }
}

void core_loop(core_t core) {
    
}


core_state_t core_get_state(core_t c) {
    return c->state;
}

