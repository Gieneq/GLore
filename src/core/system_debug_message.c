#include "system_debug_message.h"

void system_debug_message_printall(world_t* world, player_t* player) {
    printf("World:\n");
    world_print_all(world);
    printf("Player: %s, c_room: %d, c_conversation: %d.\n", player->name, player->current_room_id, player->current_conversation_npc_id);
}