#include "room.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "npc.h"

result_t room_append_adjecent_room(room_t* room, int adjecent_room) {
    if(room->adjecent_rooms_count >= ROOM_ADJECTENT_ROOMS_MAX_COUNT) {
        printf("Too many adjecent rooms.\n");
        return RESULT_ERROR;
    }

    if(adjecent_room == INVALID_ID) {
        printf("Adjecent room data corrupted.\n");
        return RESULT_ERROR;
    }

    if(room->id == adjecent_room) {
        printf("Cannot selfconnect room %d.\n", room->id);
        return RESULT_ERROR;
    }

    int new_adjecent_room_index = room->adjecent_rooms_count;
    room->adjecent_rooms_ids[new_adjecent_room_index] = adjecent_room;

    ++(room->adjecent_rooms_count);
    return RESULT_OK;
}

result_t room_init(room_t* room) {
    debug_printf("Room initializing\n");

    if(room_clear(room) != RESULT_OK) {
        printf("Cannot clear room data.\n");
        return RESULT_ERROR;
    }

    return RESULT_OK;
}

result_t room_clear(room_t* room) {
    if(!room) {
        printf("Room data corrupted.\n");
        return RESULT_ERROR;
    }

    /* ID */
    room->id = 0;

    /* Name */
    memset(room->name, '\0', ROOM_NAME_BUFFER_SIZE);

    /* Adjecent rooms */
    for(int i=0; i<ROOM_ADJECTENT_ROOMS_MAX_COUNT; ++i) {
        room->adjecent_rooms_ids[i] = INVALID_ID;
        room->adjecent_rooms_count = 0;
    }

    /* NPCs list */
    memset(room->npcs_ids, 0, sizeof(int) * ROOM_NPCS_MAX_COUNT);
    room->npcs_count = 0;
    
    return RESULT_OK;
}

bool_t room_is_valid(room_t* room) {
    return room->id != 0 && strlen(room->name) > 0 ? BOOL_TRUE : BOOL_FALSE;
}

result_t room_append_npc_id(room_t* room, int npc_id) {
    /* Validate space and npc data */
    if(room->npcs_count >= ROOM_NPCS_MAX_COUNT) {
        printf("Too many NPCs in the room.\n");
        return RESULT_ERROR;
    }

    if(npc_id == INVALID_ID) {
        printf("NPC id invalid.\n");
        return RESULT_ERROR;
    }

    if(room_has_npc_with_id(room, npc_id) == OPTION_SOME) {
        printf("NPC with id %d already in the room.\n", npc_id);
        return RESULT_ERROR;
    }

    /* Store NPC id */
    int new_npc_index = room->npcs_count;
    room->npcs_ids[new_npc_index] = npc_id;
    ++(room->npcs_count);
    return RESULT_OK;
}

option_t room_has_npc_with_id(room_t* room, int npc_id) {
    for(int i=0; i<room->npcs_count; ++i) {
        if(room->npcs_ids[i] == npc_id) {
            return OPTION_SOME;
        }
    }
    return OPTION_NONE;
}

result_t room_set_name(room_t* room, const char* name) {
    if(strlen(name) > ROOM_NAME_MAX_LENGTH) {
        printf("New room name too long.\n");
        return RESULT_ERROR;
    }

    strcpy(room->name, name);
    return RESULT_OK;
}

bool_t room_has_adjecent_vacancy(room_t* room) {
    return room->adjecent_rooms_count < ROOM_ADJECTENT_ROOMS_MAX_COUNT ? BOOL_TRUE : BOOL_FALSE;
}

