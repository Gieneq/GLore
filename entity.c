#include "entity.h"
#include <stdlib.h>
#include <string.h>

/*
Item
*/
struct item {
    int value;
    int quantity;
    int quantity2;
};

result_t item_create(item_t *item) {
    *item = malloc(sizeof(item_t));
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
struct player {
    char name[PLAYER_NAME_MAX];
    int health;
    int stamina;
};

result_t player_create(player_t *player, const char* p_name) {
    *player = malloc(sizeof(player_t));
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