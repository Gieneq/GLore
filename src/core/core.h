#pragma once
#include <stdlib.h>
#include "utils.h"
#include "database.h"
#include "world.h"

#define CORE_ARG_BUFFER_SIZE 128
#define CORE_ARG_MAX_LENGTH (CORE_ARG_BUFFER_SIZE-1)

typedef enum core_state {
    CORE_STATE_RUNNING,
    CORE_STATE_STOPPED,
} core_state_t;

typedef struct core_t {
    core_state_t state;
    char arg_buffer[CORE_ARG_BUFFER_SIZE];
    size_t arg_buffer_index;

    world_t world;
} core_t;

result_t core_create(core_t *core);
result_t core_delete(core_t* core);
result_t core_populate(core_t* core, database_t* database);
void core_input(core_t* core);
void core_loop(core_t* core);
core_state_t core_get_state(core_t* core);



