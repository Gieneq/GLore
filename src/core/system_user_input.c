#include "system_user_input.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "npc.h"
#include "room.h"
#include "dialog.h"
#include "keywords.h"
#include "core.h"
#include "world.h"
#include "system_player.h"
#include "system_dialogs.h"
#include "system_help.h"
#include "system_debug_message.h"

static help_t help;

static word_iterator_t word_split_iterator;
static keywords_list_t kws_exit;
static keywords_list_t kws_whoemi;
static keyword_t kw_help;
static keyword_t kw_look;
static keyword_t kw_go;
static keyword_t kw_hint;
static keyword_t kw_debug;
static keyword_t kw_questlog;

static option_t system_user_input_general(core_t* core, player_t* player, const char* msg) {
    /* Quit */
    if(keywords_list_match_any_ignorecase(&kws_exit, msg) == OPTION_SOME) {
        info_printf("Quitting, bye then.\n");
        core->state = CORE_STATE_STOPPED;
        return OPTION_SOME;
    }

    /* Debug */
    if(keyword_match_front_ignorecase(&kw_debug, msg) == OPTION_SOME) {
        system_debug_message_printall(&core->world, player);
        return OPTION_SOME;
    }

    /* Help */
    if(keyword_match_front_ignorecase(&kw_help, msg) == OPTION_SOME) {
        system_help_print(&help);
        return OPTION_SOME;
    }

    /* Who em I */
    if(keywords_list_match_any_ignorecase(&kws_whoemi, msg) == OPTION_SOME) {
        info_printf("You are %s\n", player->name);
        return OPTION_SOME;
    }

    /* Hint - what command can I use now */
    if(keyword_match_any_ignorecase(&kw_hint, msg) == OPTION_SOME) {
        /* Check if player in conversation. If not write information about saying 'hi', */
        /* else print all kewords for specyfic NPC dialog stage. */
        system_help_print_hint(&core->world, player);
        return OPTION_SOME;
    }

    if(keyword_match_any_ignorecase(&kw_questlog, msg) == OPTION_SOME) {
        questlog_print(&player->questlog);
        return OPTION_SOME;
    }

    return OPTION_NONE;
}

static option_t system_user_input_examine_room(world_t* world, player_t* player, room_t* current_room, const char* msg) {
    /* Look */
    if(keyword_match_front_ignorecase(&kw_look, msg) == OPTION_SOME) {
        info_printf("You are in %s ", current_room->name);
        if(current_room->npcs_count == 0) {
            info_printf("noone here.\n");
            return OPTION_SOME;
        }

        /* Some NPCs inside room */
        info_printf("and see %d NPCs:\n", current_room->npcs_count);
        for(int i=0; i<current_room->npcs_count; ++i) {
            const char* npc_name = NULL;
            const int npc_id = current_room->npcs_ids[i];
            world_get_npc_name_by_id_or_unknown(world, &npc_name, npc_id);
#if DEBUG == 1
            info_printf(" * %s (%d)\n", npc_name, npc_id);
#else
            info_printf(" * %s\n", npc_name);
#endif
        }

        return OPTION_SOME;
    }
    return OPTION_NONE;
}

static option_t system_user_input_manipulate_items(player_t* player, room_t* current_room, const char* msg) {
    return OPTION_NONE;
}

static option_t system_user_input_player_go(world_t* world, player_t* player, room_t* current_room, const char* msg) {
    word_iterator_start(&word_split_iterator, msg);

    if(word_iterator_has_next(&word_split_iterator) == OPTION_SOME) {
        const char* word = word_split_iterator.next_word;
        const char* remaining = word_split_iterator.next_word_src;
        const int word_index = word_split_iterator.next_word_index;

        /* Check if first word matches keword. Checking index is redundant. */
        if(word_index == 0 && keyword_match_front_ignorecase(&kw_go, word) == OPTION_SOME) {
            if(current_room->adjecent_rooms_count == 0) {
                info_printf("You can't go anywhere from here.\n");
                return OPTION_SOME;
            }

            /* Single keyword */
            if(strlen(remaining) == 0) {
                /* Iterate over list of adjecent rooms */
                info_printf("You can go to:\n");

                for(int i=0; i<current_room->adjecent_rooms_count; i++) {
                    const int adjecent_room_id = current_room->adjecent_rooms_ids[i];
                    const char* adjecent_room_name;
                    world_get_room_name_by_id_or_unknown(world, &adjecent_room_name, adjecent_room_id);
#if DEBUG == 1
                    info_printf(" * %s (%d)\n", adjecent_room_name, adjecent_room_id);
#else
                    info_printf(" * %s \n", adjecent_room_name);
#endif
                }
                return OPTION_SOME;
            }

            /* Keyword with remaining direction */
            for(int i=0; i<current_room->adjecent_rooms_count; i++) {
                // room_t* adjecent_room = current_room->adjecent_rooms[i];
                const int adjecent_room_id = current_room->adjecent_rooms_ids[i];
                const char* adjecent_room_name;
                world_get_room_name_by_id_or_unknown(world, &adjecent_room_name, adjecent_room_id);
                if(string_equals_ignorecase(adjecent_room_name, remaining) == OPTION_SOME) {
                    //todo - possible unsafe method to change room
                    system_player_change_room(world, player, adjecent_room_id);
                    return OPTION_SOME;
                }
            }
            info_printf("There is no road leading to %s.\n", remaining);
            return OPTION_SOME;
        }
    }
    return OPTION_NONE;
}

static option_t system_user_input_npc_interaction(world_t* world, player_t* player, room_t* current_room, const char* msg) {
    /* Try interacting with NPCs */
    /* Iterate over all NPCs in current room. You don't know what dialog state they have.  */
    /* Try matching their dialog blocks, return on success. */
    for(int i=0; i<current_room->npcs_count; ++i) {
        const int npc_id = current_room->npcs_ids[i];
        npc_t* selected_npc = NULL;
        if(world_get_npc_by_id(world, &selected_npc, npc_id) != OPTION_SOME) {
            error_printf("Seems NPC %d is corrupted.\n", npc_id);
            return OPTION_NONE;
        }

        /* Try matching NPCs dialogs blocks */
        if(system_dialog_match_user_input(selected_npc, player, msg) == OPTION_SOME) {
            return OPTION_SOME;
        }
    }

    return OPTION_NONE;
}

static void system_user_input_on_unknown() {
    debug_printf("What?\n");
}

/* Public interface */
void system_user_input_init() {
    /* Help */
    system_help_init(&help);
    system_help_set_initial_text(&help, "You can navigate and communicate with NPC using text. \nOnly keywords matches. Common ignorecase commands:");
    system_help_set_ending_text(&help, "Dialogs with NPCs has no pattern.");


    /* Keywords */
    keyword_from_string(&kw_help, "!help"); system_help_append_command_with_keyword(&help, &kw_help, "Prints this message.");
    keyword_from_string(&kw_look, "look"); system_help_append_command_with_keyword(&help, &kw_look, "Look around current room.");
    keyword_from_string(&kw_go, "go"); system_help_append_command_with_keyword(&help, &kw_go, "List possible roads. Add road name to go there.");
    keyword_from_string(&kw_hint, "hint"); system_help_append_command_with_keyword(&help, &kw_hint, "Give information about current conversation keywords.");
    keyword_from_string(&kw_debug, "!debug"); system_help_append_command_with_keyword(&help, &kw_debug, "Give some more information.");
    keyword_from_string(&kw_questlog, "ql"); system_help_append_command_with_keyword(&help, &kw_questlog, "Print entire questlog.");



    /* Keywords lists */
    keywords_list_from_delimited_string(&kws_exit, "!quit,!q,", ","); system_help_append_command_with_keywords_list(&help, &kws_exit, "Shout the game. Whithout save so far.");
    keywords_list_from_delimited_string(&kws_whoemi, "me,who em i,whoemi", ","); system_help_append_command_with_keywords_list(&help, &kws_whoemi, "Give information about player.");
    
    /* Some more help messages */
}

void system_user_input_process(core_t* core, player_t* player, const char* msg) {
    debug_printf("Player %s says \'%s\' so...\n", player->name, msg);

    /* General */
    if(system_user_input_general(core, player, msg) == OPTION_SOME) {
        return;
    } 
    
    /* Player in any room required, if not then seems bugged */
    if(player->current_room_id == INVALID_ID) {
        error_printf("Player without current room id, seems bad...\n");
        return; // nothing to do, but it should not happen
    }

    room_t* current_room = NULL;
    if(world_get_room_by_id(&core->world, &current_room, player->current_room_id) != OPTION_SOME) {
        error_printf("Players current room %d not exist, seems bad...\n", player->current_room_id);
        return; // nothing to do, but it should not happen
    }

    /* Finally! */
    {
        /* Examin room */
        if(system_user_input_examine_room(&core->world, player, current_room, msg) == OPTION_SOME ) {
            return;
        } 
        
        /* Manipulate items */
        if(system_user_input_manipulate_items(player, current_room, msg) == OPTION_SOME ) {
            return;
        } 

        /* Movement */
        if(system_user_input_player_go(&core->world, player, current_room, msg) == OPTION_SOME ) {
            return;
        } 

        /* NPC interaction */
        if(system_user_input_npc_interaction(&core->world, player, current_room, msg) == OPTION_SOME ) {
            return;
        }
    }

    /* Unknown */
    system_user_input_on_unknown();
}
