#pragma once
#include "utils.h"

#define PLAYER_NAME_MAX 32
#define PLAYER_INITIAL_HEALTH 160
#define PLAYER_INITIAL_STAMINA 20

#define NPC_NAME_MAX 32
#define NPC_INITIAL_HEALTH 360
#define NPC_INITIAL_STAMINA 80


typedef enum response {
    RESPONSE_SOME,
    RESPONSE_NONE,
} response_t;

typedef struct item_obj * item_t;
typedef struct player_obj * player_t;
typedef struct npc_obj * npc_t;


result_t item_create(item_t *item);
result_t item_delete(item_t item);

result_t player_create(player_t *player, const char* p_name);
result_t player_delete(player_t player);
void player_print(player_t player);

result_t npc_create(npc_t *npc, const char* npc_name);
result_t npc_delete(npc_t npc);
response_t npc_response(player_t player, npc_t npc, const char* message);
