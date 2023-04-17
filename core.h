#pragma once
#include "utils.h"
#include "entity.h"

typedef struct core_obj * core_t;

typedef enum core_state {
    CORE_STATE_RUNNING,
    CORE_STATE_STOPPED,
} core_state_t;

result_t core_create(core_t *core);
result_t core_delete(core_t core);
void core_input(core_t core);
void core_loop(core_t core);
core_state_t core_get_state(core_t core);



