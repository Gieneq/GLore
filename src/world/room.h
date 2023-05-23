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

    int adjecent_rooms_ids[ROOM_ADJECTENT_ROOMS_MAX_COUNT]; //pointers replaced with id for flexibility
    int adjecent_rooms_count;

    int npcs_ids[ROOM_NPCS_MAX_COUNT]; //todo - replace with id
    int npcs_count;
} room_t;

result_t room_init(room_t* room);
result_t room_clear(room_t* room);
bool_t room_is_valid(room_t* room);
result_t room_set_name(room_t* room, const char* name);

/* NPCs in room */
result_t room_append_npc_id(room_t* room, int npc_id);
option_t room_has_npc_with_id(room_t* room, int npc_id);

/* Roads */
bool_t room_has_adjecent_vacancy(room_t* room);
result_t room_append_adjecent_room(room_t* room, int adjecent_room);