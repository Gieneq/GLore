#pragma once
#include "utils.h"


typedef struct world_obj * world_t;

result_t world_create(world_t * world);
result_t world_delete(world_t world);