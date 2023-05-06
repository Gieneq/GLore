#pragma once
#include "utils.h"
#include "player.h"
#include "room.h"

#define WORLD_ROOMS_MAX_COUNT 128


typedef struct world_t {
    room_t rooms[WORLD_ROOMS_MAX_COUNT];
    int rooms_count;
    player_t player;
} world_t;

result_t world_init(world_t* world);
result_t world_append_room(world_t* world, room_t* room);
room_iter_t world_get_room_iter(world_t* world);