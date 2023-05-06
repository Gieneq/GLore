#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "world.h"



result_t world_init(world_t* world) {
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

    if(room_is_valid(room) == FALSE) {
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


// room_iter_t world_get_room_iter(world_t* world);