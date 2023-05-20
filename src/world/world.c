#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "world.h"

static const char* WORLD_UNKNOWN_ROOM_NAME = "#Unknown location";

result_t world_init(world_t* world) {
    debug_printf("World initializing\n");
    world->rooms_count = 0;
    memset(world->rooms, 0, sizeof(room_t) * WORLD_ROOMS_MAX_COUNT);
    return RESULT_OK;
}

result_t world_append_room(world_t* world, room_t* room) {
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