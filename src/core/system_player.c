#include "system_player.h"
#include "system_dialogs.h"

result_t system_player_change_room(player_t* player, room_t* room) {
    if(!player || !room) {
        printf("Player or room is missing or corrupted.");
        return RESULT_ERROR;
    }
    room_t* current_room = player->current_room;

    if(current_room == room) {
        //todo in future change to separated system for printing
        printf("You are already in this room.");
        return RESULT_ERROR;
    }



    /* Start leaving room */
    if(current_room) {
        printf("You are leaving \'%s\'.\n", current_room->name);
    }

    /* Drop conversation if any */
    npc_iter_t npc_iter = room_get_npc_iter(current_room);
    debug_printf("Searching for notfinished conversations over %d NPCs in the room %s:\n", npc_iter.count, current_room->name);
    
    npc_t* selected_npc = NULL;
    iterator_foreach(&selected_npc, &npc_iter) {
        if(npc_is_in_conversation(selected_npc)) {
            debug_printf("NPC \'%s\' is in conversation. Leaving conversation.\n", selected_npc->name);
            // npc_leave_conversation(selected_npc);
            system_dialog_npc_leave_conversation(selected_npc);
        }
    }

    /* Entrer other room */
    if(room) {
        printf("You are entering \'%s\'.\n", room->name);
    }

    player->current_room = room;

    return RESULT_OK;
}