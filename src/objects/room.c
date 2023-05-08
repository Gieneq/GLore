#include "room.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "npc.h"

result_t room_init(room_t* room) {
    printf("Room initializing\n");

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

    /* NPCs list */
    memset(room->npcs, 0, sizeof(npc_t) * ROOM_NPCS_MAX_COUNT);
    room->npcs_count = 0;
    
    return RESULT_OK;
}

bool_t room_is_valid(room_t* room) {
    return room->id != 0 && strlen(room->name) > 0 ? TRUE : FALSE;
}

result_t room_append_npc(room_t* room, npc_t* npc) {
    /* Validate space and npc data */
    if(room->npcs_count >= ROOM_NPCS_MAX_COUNT) {
        printf("Too many NPCs in the room.\n");
        return RESULT_ERROR;
    }

    if(!npc) {
        printf("NPC data corrupted.\n");
        return RESULT_ERROR;
    }

    if(npc_is_valid(npc) == FALSE) {
        printf("NPC data is invalid.\n");
        return RESULT_ERROR;
    }

    /* Copy NPC data */
    int new_npc_index = room->npcs_count;
    memcpy(&room->npcs[new_npc_index], npc, sizeof(npc_t));

    /* After moving leave NPC data invalid */
    if(npc_clear(npc) != RESULT_OK) {
        printf("Cannot clear NPC data after moving.\n");
        // clear occupied memory?
        return RESULT_ERROR;
    }

    ++(room->npcs_count);
    return RESULT_OK;
}

result_t room_get_npc_by_index(room_t* room, npc_t** npc, const int index) {
    if(room->npcs_count <= 0) {
        printf("There is np NPCs in the room.\n");
        return RESULT_ERROR;
    }

    int highest_available_npc_index = room->npcs_count - 1;

    if(index < 0 || index > highest_available_npc_index) {
        printf("NPC index out of range.\n");
        return RESULT_ERROR;
    }

    *npc = room->npcs + index;
    return RESULT_OK;
}

npc_iter_t room_get_npc_iter(room_t* room) {
    npc_iter_t iter = {room->npcs, 0, room->npcs_count};
    return iter;
}

result_t room_set_name(room_t* room, const char* name) {
    if(strlen(name) > ROOM_NAME_MAX_LENGTH) {
        printf("New room name too long.\n");
        return RESULT_ERROR;
    }

    strcpy(room->name, name);
    return RESULT_OK;
}