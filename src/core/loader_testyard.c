#include "loader_testyard.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "npc.h"
#include "room.h"
#include "item.h"
#include "quest.h"
#include "questlog.h"
#include "dialog.h"
#include "keywords.h"
#include "responses.h"
#include "database.h"


// static result_t create_test


result_t loader_testyard_populate(world_t* world, database_t* database) {
    printf("Populating test-world\n");


    /* Create test rooms */
    room_t test_room_1;
    room_init(&test_room_1);
    test_room_1.id = 1;
    room_set_name(&test_room_1, "The dock");
    
    room_t test_room_2;
    room_init(&test_room_2);
    test_room_2.id = 2;
    room_set_name(&test_room_2, "Birch forest");



    /* Create test NPC */
    npc_t test_npc_1;
    npc_init(&test_npc_1);
    test_npc_1.id = 1;
    if(npc_set_name(&test_npc_1, "Stefan") == RESULT_ERROR) {
        printf("Cannot assign name to the npc.\n");
        return RESULT_ERROR;
    }
    
    npc_t test_npc_2;
    npc_init(&test_npc_2);
    test_npc_2.id = 2;
    if(npc_set_name(&test_npc_2, "Jim") == RESULT_ERROR) {
        printf("Cannot assign name to the npc.\n");
        return RESULT_ERROR;
    }
    
    npc_t test_npc_3;
    npc_init(&test_npc_3);
    test_npc_3.id = 3;
    if(npc_set_name(&test_npc_3, "Bim") == RESULT_ERROR) {
        printf("Cannot assign name to the npc.\n");
        return RESULT_ERROR;
    }


    /* Fill up rooms */
    if(room_append_npc(&test_room_1, &test_npc_1) == RESULT_ERROR) {
        printf("Cannot append npc to the room.\n");
        return RESULT_ERROR;
    }
    
    if(room_append_npc(&test_room_1, &test_npc_2) == RESULT_ERROR) {
        printf("Cannot append npc to the room.\n");
        return RESULT_ERROR;
    }
    
    if(room_append_npc(&test_room_1, &test_npc_3) == RESULT_ERROR) {
        printf("Cannot append npc to the room.\n");
        return RESULT_ERROR;
    }


    /* Some tests about NPCs */
    npc_t* some_npc = NULL;
    if(room_get_npc_by_index(&test_room_1, &some_npc, 2) != RESULT_OK) {
        printf("Missing NPC with specyfic index in array.\n");
        return RESULT_ERROR;
    }
    printf("Got name: %s\n", some_npc->name);


    /* Add rooms to world */
    if(world_append_room(world, &test_room_1) == RESULT_ERROR) {
        printf("Cannot append room to the world.\n");
        return RESULT_ERROR;
    }

    if(world_append_room(world, &test_room_2) == RESULT_ERROR) {
        printf("Cannot append room to the world.\n");
        return RESULT_ERROR;
    }


    /* Iterate over all rooms and all NPCs inside them */
    room_iter_t room_iter = world_get_room_iter(world);
    printf("Iterating over %d rooms in the world:\n", room_iter.count);

    room_t* a_room = NULL;    
    iterator_foreach(&a_room, &room_iter) {
        npc_iter_t npc_iter = room_get_npc_iter(a_room);
        printf(" * iterating over %d NPCs in the room %s:\n", npc_iter.count, a_room->name);
        
        npc_t* a_npc = NULL;
        iterator_foreach(&a_npc, &npc_iter) {
            printf("   - %d NPC: %s,\n", npc_iter.current, a_npc->name);
        }
    }



    return RESULT_OK;
}