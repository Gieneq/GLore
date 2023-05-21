#include "system_player.h"
#include "system_dialogs.h"

result_t system_player_change_room(world_t* world, player_t* player, int room_id) {
    if(!world || !player) {
        printf("World or player missing or corrupted.");
        return RESULT_ERROR;
    }
    const int previous_room_id = player->current_room_id;

    // blah blah, if current room not exists thats fine!!!!!!!!!!!!!!!!!!!1

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

        /* Drop conversation if any */
        
        // printf(">>>>>todo Drop conversation if any \n");
        // const room_t* current_room_data = NULL;
        // world_get_room_by_id()

        // npc_iter_t npc_iter = room_get_npc_iter(previous_room);
        // debug_printf("Searching for notfinished conversations over %d NPCs in the room %s:\n", npc_iter.count, previous_room->name);
        
        // iterator_foreach(&selected_npc, &npc_iter) {
        for(int i=0; i<previous_room->npcs_count; ++i) {
            const int selected_npc_id = previous_room->npcs_ids[i];
            npc_t* selected_npc = NULL;
            if(world_get_npc_by_id(world, &selected_npc, selected_npc_id) != OPTION_SOME) {
                debug_printf("Not found NPC. Should not happen, blah...\n");
                return RESULT_ERROR;
            }
            if(npc_is_in_conversation(selected_npc)) {
                debug_printf("NPC \'%s\' is in conversation. Leaving conversation.\n", selected_npc->name);
                system_dialog_npc_leave_conversation(selected_npc);
            }
        }
    }

    /* Finally entrer other room */
    printf("You are entering \'%s\'.\n", other_room->name);
    player->current_room_id = room_id;
    return RESULT_OK;
}