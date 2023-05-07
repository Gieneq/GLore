#include "system_player_communication.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "npc.h"
#include "room.h"
#include "dialog.h"
#include "keywords.h"
#include "core.h"

static keyword_t kw_exit;
static keyword_t kw_help;

static option_t system_player_communication_want_quit(player_t* player, const char* msg) {
    return keyword_match_front(&kw_exit, msg);
}

static option_t system_player_communication_want_help(player_t* player, const char* msg) {
    return keyword_match_front(&kw_help, msg);
}


void system_player_communication_init() {
    keyword_from_string(&kw_exit, "!exit");
    keyword_from_string(&kw_help, "!help");
}

void system_player_communication_process(core_t* core, player_t* player, const char* msg) {
    printf("Player %s says \'%s\' so...\n", player->name, msg);

    if(system_player_communication_want_help(player, msg) == OPTION_SOME) {
        printf("Help yourself?\n");
    } else if(system_player_communication_want_quit(player, msg) == OPTION_SOME) {
        printf("Quitting?\n");
        core->state = CORE_STATE_STOPPED;
    } else {
        printf("What?\n");
    }
}



// static void _core_process_cmd(core_t* core) {
//     if (strcmp(core->arg_buffer, "quit") == 0) {
//         core->state = CORE_STATE_STOPPED;
//     } 
    
//     else if (strcmp(core->arg_buffer, "help") == 0) {
//         printf("Commands:\n");
//         printf("quit - quit the game\n");
//         printf("help - show this help\n");
//     } 

//     else {
//         printf("Unknown command: %s\n", core->arg_buffer);
//     }

//     memset(core->arg_buffer, '\0', CORE_ARG_BUFFER_SIZE);
//     core->arg_buffer_index = 0;
// }