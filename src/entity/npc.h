#pragma once 
#include "utils.h"
#include "dialog.h"

#define NPC_NAME_BUFFER_SIZE 32
#define NPC_NAME_MAX_LENGTH (NPC_NAME_BUFFER_SIZE - 1)

#define NPC_DIALOG_BLOCKS_MAX_COUNT 32

typedef struct npc_t {
    int id;
    char name[NPC_NAME_BUFFER_SIZE];
    int dialog_stage;
    dialog_block_t dialog_blocks[NPC_DIALOG_BLOCKS_MAX_COUNT];
    int dialog_blocks_count;
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
result_t npc_append_dialog_block(npc_t* npc, dialog_block_t* dialog_block);