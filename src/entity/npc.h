#pragma once 
#include "utils.h"

#define NPC_NAME_BUFFER_SIZE 32
#define NPC_NAME_MAX_LENGTH (NPC_NAME_BUFFER_SIZE - 1)


typedef struct npc_t {
    int id;
    char name[NPC_NAME_BUFFER_SIZE];
    int dialog_stage;
} npc_t;

typedef struct npc_iter_t {
    npc_t* first;
    int current;
    int count;
} npc_iter_t;


result_t npc_init(npc_t* npc);
result_t npc_clear(npc_t* npc);
bool_t npc_is_valid(npc_t* npc);
result_t npc_set_name(npc_t* npc, const char* name);