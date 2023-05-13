#pragma once
#include "utils.h"
#include "npc.h"


#define ROOM_NAME_BUFFER_SIZE 32
#define ROOM_NAME_MAX_LENGTH (ROOM_NAME_BUFFER_SIZE - 1)

#define ROOM_NPCS_MAX_COUNT 16
#define ROOM_ADJECTENT_ROOMS_MAX_COUNT 8

typedef struct room_t {
    int id;
    char name[ROOM_NAME_BUFFER_SIZE];

    struct room_t* adjecent_rooms[ROOM_ADJECTENT_ROOMS_MAX_COUNT];
    int adjecent_rooms_count;

    npc_t npcs[ROOM_NPCS_MAX_COUNT];
    int npcs_count;
} room_t;

typedef struct room_iter_t {
    room_t* first;
    int current;
    int count;
} room_iter_t;


result_t room_init(room_t* room);
result_t room_clear(room_t* room);
bool_t room_is_valid(room_t* room);
result_t room_append_npc(room_t* room, npc_t* npc);
result_t room_get_npc_by_index(room_t* room, npc_t** npc, const int index);
npc_iter_t room_get_npc_iter(room_t* room);
result_t room_set_name(room_t* room, const char* name);
bool_t room_has_adjecent_vacancy(room_t* room);
result_t room_connect_bidirectional(room_t* room_1, room_t* room_2);
room_iter_t room_get_adjecent_room_iter(room_t* room);