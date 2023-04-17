#include "entity.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
Item
*/
struct item_obj {
    int value;
    int quantity;
    int quantity2;
};

result_t item_create(item_t *item) {
    *item = malloc(sizeof(struct item_obj));
    if (*item == NULL) {
        return RESULT_ERROR;
    }
    (*item)->value = 0;
    (*item)->quantity = 0;
    return RESULT_OK;
}

result_t item_delete(item_t item) {
    if (item == NULL) {
        return RESULT_ERROR;
    }
    
    free(item);
    return RESULT_OK;
}

/*
Player
*/
struct player_obj {
    char name[PLAYER_NAME_MAX];
    int health;
    int stamina;
};

result_t player_create(player_t *player, const char* p_name) {
    *player = malloc(sizeof(struct player_obj));
    printf("Allocationg player %llu B.\n", sizeof(struct player_obj));
    if (*player == NULL) {
        return RESULT_ERROR;
    }

    memset((*player)->name, '\0', PLAYER_NAME_MAX);
    size_t name_len = MIN(strlen(p_name), PLAYER_NAME_MAX - 1);
    strncpy((*player)->name, p_name, name_len);
    
    (*player)->health = PLAYER_INITIAL_HEALTH;
    (*player)->stamina = PLAYER_INITIAL_STAMINA;
    return RESULT_OK;
}

void player_print(player_t player) {
    printf("Player: %s\n", player->name);
    printf("Health: %d\n", player->health);
    printf("Stamina: %d\n", player->stamina);
}

result_t player_delete(player_t player) {
    if (player == NULL) {
        return RESULT_ERROR;
    }
    
    free(player);
    return RESULT_OK;
}


/*
NPC
*/
struct npc_obj {
    char name[NPC_NAME_MAX];
    int health;
    int stamina;
};

result_t npc_create(npc_t *npc, const char* npc_name) {
    *npc = malloc(sizeof(struct npc_obj));
    if (*npc == NULL) {
        return RESULT_ERROR;
    }

    memset((*npc)->name, '\0', NPC_NAME_MAX);
    size_t name_len = MIN(strlen(npc_name), NPC_NAME_MAX - 1);
    strncpy((*npc)->name, npc_name, name_len);
    
    (*npc)->health = NPC_INITIAL_HEALTH;
    (*npc)->stamina = NPC_INITIAL_STAMINA;
    return RESULT_OK;
}

result_t npc_delete(npc_t npc) {
    if (npc == NULL) {
        return RESULT_ERROR;
    }
    
    free(npc);
    return RESULT_OK;
}

response_t npc_response(player_t player, npc_t npc, const char* message) {
    if(strcmp(message, "hi") == 0) {
        printf("%s said: Hello %s!\n", npc->name, player->name);
        return RESPONSE_SOME;
    }
    else if(strcmp(message, "bye") == 0) {
        printf("%s said: See you again %s!\n", npc->name, player->name);
        return RESPONSE_SOME;
    }

    printf("%s said: I don't understand you %s!\n", npc->name, player->name);
    return RESPONSE_NONE;
}