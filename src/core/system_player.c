#include "system_player.h"
#include "system_dialogs.h"
#include "system_items.h"

result_t system_player_change_room(world_t* world, player_t* player, int room_id) {
    if(!world || !player) {
        printf("World or player missing or corrupted.");
        return RESULT_ERROR;
    }
    const int previous_room_id = player->current_room_id;

    if(room_id == INVALID_ID) {
        error_printf("Error: Invalid room id\n");
        return RESULT_ERROR;
    }

    /* Case 1: passed the same id as current id */
    if(previous_room_id == room_id) {
        //todo in future change to separated system for printing
        error_printf("Error: You are already in this room.\n");
        return RESULT_OK; //it is fine, not breaking system
    }

    /* Case 2: passed other id, check if exists */

    room_t* other_room = NULL;
    if(world_get_room_by_id(world, &other_room, room_id) != OPTION_SOME) {
        error_printf("Error: there is no room data with id %d!\n", room_id);
        return RESULT_ERROR;
    }

    /* Case 2A: player wasn't in any room before */
    if(previous_room_id == INVALID_ID) {
        ; // nothing
    }

    /* Case 2B: player was in some room before */
    else {
        room_t* previous_room = NULL;
        if(world_get_room_by_id(world, &previous_room, previous_room_id) != OPTION_SOME) {
            error_printf("Error: there is no room data with id %d!\n", previous_room_id);
            return RESULT_ERROR;
        }
        printf("You are leaving \'%s\'.\n", previous_room->name); 

        /* Drop conversation with current NPC */
        npc_t* npc_data = NULL;
        if(system_player_get_npc_in_conversation(world, player, &npc_data) != OPTION_NONE) {
            debug_printf("NPC is in conversation. Leaving conversation.\n");
            system_dialog_npc_leave_conversation(npc_data, player);
        }
    }

    /* Finally entrer other room */
    printf("You are entering \'%s\'.\n", other_room->name);
    player->current_room_id = room_id;
    return RESULT_OK;
}

option_t system_player_get_npc_in_conversation(world_t* world, player_t* player, npc_t** npc_data) {
    if(player_is_in_conversation(player) == BOOL_FALSE) {
        *npc_data = NULL;
        return OPTION_NONE;
    }
    
    npc_t* npc_with_given_id = NULL;
    if(world_get_npc_by_id(world, &npc_with_given_id, player->current_conversation_npc_id) != OPTION_SOME) {
        error_printf("Error: there is no npc data with id %d!\n", player->current_conversation_npc_id);
        return OPTION_NONE;
    }

    if((npc_is_in_conversation(npc_with_given_id) == BOOL_FALSE) && (player->current_conversation_npc_id == npc_with_given_id->id)) {
        error_printf("Error: some heavy logic error. Selected NPC %d should be the same as in conversation %d!\n", npc_with_given_id->id, player->current_conversation_npc_id);
        *npc_data = NULL;
        return OPTION_NONE;
    }

    *npc_data = npc_with_given_id;
    return OPTION_SOME;
}

void system_player_print_player(player_t* player, item_database_t* item_database) {
    info_printf("You are %s\n", player->name);
    system_items_container_print(item_database, &player->backpack, "\n");
}