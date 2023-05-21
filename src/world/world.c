#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "world.h"

static const char* WORLD_UNKNOWN_ROOM_NAME = "#Unknown location";
static const char* WORLD_UNKNOWN_NPC_NAME = "#Unknown npc";

result_t world_init(world_t* world) {
    debug_printf("World initializing\n");
    world->rooms_count = 0;
    memset(world->rooms, 0, sizeof(room_t) * WORLD_ROOMS_MAX_COUNT);
    memset(world->npcs_list, 0, sizeof(npc_t) * WORLD_NPC_MAX_COUNT);
    world->npcs_count = 0;
    for(int i=0; i< WORLD_NPC_MAX_COUNT; ++i) {
        if(npc_clear(&world->npcs_list[i]) != RESULT_OK) {
            return RESULT_ERROR;
        }
    }

    return RESULT_OK;
}


void world_print_all(world_t* world) {
    printf("World print all:\n");

    printf(" NPCs count %d:\n", world->npcs_count);
    for(int i=0; i<world->npcs_count; ++i) {
        printf(" * %s (%d)\n", world->npcs_list[i].name, world->npcs_list[i].id);
    }

    printf(" Rooms count %d:\n", world->rooms_count);
    for(int i=0; i<world->rooms_count; ++i) {
        const room_t* a_room = &world->rooms[i];
        printf(" * %s (%d), NPCs: ", a_room->name, a_room->id);
        for(int j=0; j<a_room->npcs_count; ++j) {
            printf("%d, ", a_room->npcs_ids[j]);
        }
        printf(", roads: ");
        for(int j=0; j<a_room->adjecent_rooms_count; ++j) {
            printf("%d, ", a_room->adjecent_rooms_ids[j]);
        }
        printf("\n");
    }
}

/* Rooms */

result_t world_append_room(world_t* world, room_t* room) {
    printf("Append room %s with id %d.\n", room->name, room->id);
    if(world->rooms_count >= WORLD_ROOMS_MAX_COUNT) {
        printf("Failed to add room to world: world is full\n");
        return RESULT_ERROR;
    }

    if(!room) {
        printf("Room data corrupted.\n");
        return RESULT_ERROR;
    }

    if(room_is_valid(room) == BOOL_FALSE) {
        printf("Room data is invalid.\n");
        return RESULT_ERROR;
    }

    /* Copy room data */
    int new_room_index = world->rooms_count;
    memcpy(&world->rooms[new_room_index], room, sizeof(room_t));

    /* After moving leave room data invalid */
    if(room_clear(room) != RESULT_OK) {
        printf("Cannot clear room data after moving.\n");
        // clear occupied memory?
        return RESULT_ERROR;
    }

    ++(world->rooms_count);
    return RESULT_OK;
}

room_iter_t world_get_room_iter(world_t* world) {
    room_iter_t iter = {world->rooms, 0, world->rooms_count};
    return iter;
}

option_t world_get_room_by_name_ignorecase(world_t* world, room_t** room, const char* name) {
    room_iter_t iter = world_get_room_iter(world);
    iterator_foreach(room, &iter) {
        if(string_equals_ignorecase((*room)->name, name) == OPTION_SOME) {
            return OPTION_SOME;
        }
    }    
    *room = NULL;
    return OPTION_NONE;
}

option_t world_get_room_id_by_name_ignorecase(world_t* world, int* room_id, const char* name) {
    room_t* room = NULL;
    room_iter_t iter = world_get_room_iter(world);
    iterator_foreach(&room, &iter) {
        if(string_equals_ignorecase(room->name, name) == OPTION_SOME) {
            *room_id = room->id;
            return OPTION_SOME;
        }
    }    
    return OPTION_NONE;
}

option_t world_has_room_with_id(world_t* world, int id) {
    if(id == INVALID_ID) {
        return OPTION_NONE;
    }
    room_t* room = NULL;
    room_iter_t iter = world_get_room_iter(world);
    iterator_foreach(&room, &iter) {
        // for(int i=0; i<world->rooms_count; ++i) { 
        // room_t* room = &world->rooms[i];
        // printf(" Looking at id: %d cmp with new %d\n", room->id, id);
        if(room->id == id) {
            return OPTION_SOME;
        }
    }
    return OPTION_NONE;
}

option_t world_get_room_name_by_id_or_unknown(world_t* world, const char** room_name, const int id) {
    if(id == INVALID_ID) {
        *room_name = WORLD_UNKNOWN_ROOM_NAME;
        return OPTION_NONE;
    }

    room_t* some_room = NULL;
    if(world_get_room_by_id(world, &some_room, id) == OPTION_SOME) {
        *room_name = some_room->name;
        return OPTION_SOME;
    }

    *room_name = WORLD_UNKNOWN_ROOM_NAME;
    return OPTION_NONE;
}

option_t world_get_room_by_id(world_t* world, room_t** room, int id) {
    if(id == INVALID_ID) {
        return OPTION_NONE;
    }
    room_iter_t iter = world_get_room_iter(world);
    iterator_foreach(room, &iter) {
        if((*room)->id == id) {
            return OPTION_SOME;
        }
    }
    *room = NULL;
    return OPTION_NONE;
}

/* NPCs */
result_t world_append_npc(world_t* world, npc_t* npc_data) {
    if(world->npcs_count >= WORLD_NPC_MAX_COUNT) {
        printf("Failed to add NPC data to world: world is full\n");
        return RESULT_ERROR;
    }

    if(!npc_data) {
        printf("NPC data corrupted.\n");
        return RESULT_ERROR;
    }

    /* Check if NPC id already in world data */
    if(world_has_npc_by_id(world, npc_data->id) == OPTION_SOME) {
        printf("NPC with id %d already in world\n", npc_data->id);
        return RESULT_ERROR;
    }

    /* Copy NPC data */
    int new_npc_index = world->npcs_count;
    memcpy(&world->npcs_list[new_npc_index], npc_data, sizeof(npc_t));

    /* After moving leave NPC data invalid */
    if(npc_clear(npc_data) != RESULT_OK) {
        printf("Cannot clear NPC data after moving.\n");
        return RESULT_ERROR;
    }

    ++(world->npcs_count);
    return RESULT_OK;
}

option_t world_has_npc_by_id(world_t* world, int npc_id) {
    for(int i=0; i<world->npcs_count; ++i) {
        if(world->npcs_list[i].id == npc_id) {
            return OPTION_SOME;
        }
    }
    return OPTION_NONE;
}

option_t world_get_npc_by_id(world_t* world, npc_t** npc, const int npc_id) {
    for(int i=0; i<world->npcs_count; ++i) {
        if(world->npcs_list[i].id == npc_id) {
            *npc = &world->npcs_list[i];
            return OPTION_SOME;
        }
    }
    *npc = NULL;
    return OPTION_NONE;
}

option_t world_get_npc_name_by_id_or_unknown(world_t* world, const char** npc_name, const int npc_id) {
    if(npc_id == INVALID_ID) {
        *npc_name = WORLD_UNKNOWN_NPC_NAME;
        return OPTION_NONE;
    }

    npc_t* some_npc = NULL;
    if(world_get_npc_by_id(world, &some_npc, npc_id) == OPTION_SOME) {
        *npc_name = some_npc->name;
        return OPTION_SOME;
    }

    *npc_name = WORLD_UNKNOWN_NPC_NAME;
    return OPTION_NONE;
}