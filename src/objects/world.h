#pragma once
#include "utils.h"
#include "player.h"

typedef struct world_t {
    player_t player;
} world_t;

result_t world_create(world_t* world);