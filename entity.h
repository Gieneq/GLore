#pragma once
#include "utils.h"

#define PLAYER_NAME_MAX 32
#define PLAYER_INITIAL_HEALTH 160
#define PLAYER_INITIAL_STAMINA 20

typedef struct item * item_t;
typedef struct player * player_t;


result_t item_create(item_t *item);
result_t item_delete(item_t item);

result_t player_create(player_t *player, const char* p_name);
result_t player_delete(player_t player);
void player_print(player_t player);