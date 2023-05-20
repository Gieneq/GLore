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

typedef struct room_iter_t {
    room_t* first;
    int current;
    int count;
} room_iter_t;

result_t world_init(world_t* world);
result_t world_append_room(world_t* world, room_t* room);
option_t world_get_room_id_by_name_ignorecase(world_t* world, int* room_id, const char* name);
option_t world_get_room_by_name_ignorecase(world_t* world, room_t** room, const char* name);
option_t world_get_room_by_id(world_t* world, room_t** room, int id);
option_t world_has_room_with_id(world_t* world, int id);
option_t world_get_room_name_by_id_or_unknown(world_t* world, const char** room_name, const int id);
room_iter_t world_get_room_iter(world_t* world);