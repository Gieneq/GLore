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

static word_iterator_t word_split_iterator;
static keywords_list_t kws_exit;
static keywords_list_t kws_whoemi;
static keyword_t kw_help;
static keyword_t kw_look;
static keyword_t kw_go;

static option_t system_user_input_general(core_t* core, player_t* player, const char* msg) {
    /* Quit */
    if(keywords_list_match_any_ignorecase(&kws_exit, msg) == OPTION_SOME) {
        info_printf("Quitting, bye then.\n");
        core->state = CORE_STATE_STOPPED;
        return OPTION_SOME;
    }

    /* Help */
    if(keyword_match_front_ignorecase(&kw_help, msg) == OPTION_SOME) {
        info_printf("Help? Blah! Help yourself dude :p\n");
        return OPTION_SOME;
    }

    /* Who em I */
    if(keywords_list_match_any_ignorecase(&kws_whoemi, msg) == OPTION_SOME) {
        info_printf("You are %s\n", player->name);
        return OPTION_SOME;
    }

    return OPTION_NONE;
}

static option_t system_user_input_examine_room(player_t* player, room_t* current_room, const char* msg) {
    /* Look */
    if(keyword_match_front_ignorecase(&kw_look, msg) == OPTION_SOME) {
        info_printf("You are in %s ", current_room->name);
        if(current_room->npcs_count == 0) {
            info_printf("noone here.\n");
            return OPTION_SOME;
        }

        /* Some NPCs inside room */
        info_printf("and see %d NPCs:\n", current_room->npcs_count);
        
        npc_iter_t npc_iter = room_get_npc_iter(current_room);
        npc_t* selected_npc = NULL;
        iterator_foreach(&selected_npc, &npc_iter) {
            info_printf(" * %s\n", selected_npc->name);
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
                    info_printf(" * %s\n", adjecent_room_name);
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

static option_t system_user_input_npc_interaction(player_t* player, room_t* current_room, const char* msg) {
    /* Try interacting with NPCs */
    /* Iterate over all NPCs in current room. You don't know what dialog state they have.  */
    /* Try matching their dialog blocks, return on success. */

    npc_iter_t npc_iter = room_get_npc_iter(current_room);
    debug_printf("Iterating over %d NPCs in the room %s:\n", npc_iter.count, current_room->name);
    
    npc_t* selected_npc = NULL;
    iterator_foreach(&selected_npc, &npc_iter) {
        debug_printf("   - %d NPC: %s,\n", npc_iter.current, selected_npc->name);

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
    /* Keywords */
    keyword_from_string(&kw_help, "!help");
    keyword_from_string(&kw_look, "look");
    keyword_from_string(&kw_go, "go");

    /* Keywords lists */
    keywords_list_from_delimited_string(&kws_exit, "!exit,!quit,!e,!q,!leave", ",");
    keywords_list_from_delimited_string(&kws_whoemi, "me,who em i,whoemi", ",");
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
        if(system_user_input_examine_room(player, current_room, msg) == OPTION_SOME ) {
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
        if(system_user_input_npc_interaction(player, current_room, msg) == OPTION_SOME ) {
            return;
        }
    }

    /* Unknown */
    system_user_input_on_unknown();
}
