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
#include "debug.h"

static word_iterator_t word_split_iterator;
static keywords_list_t kws_exit;
static keyword_t kw_help;
static keyword_t kw_look;
static keyword_t kw_go;

static option_t system_user_input_general(core_t* core, player_t* player, const char* msg) {
    /* Quit */
    if(keywords_list_match_front(&kws_exit, msg) == OPTION_SOME) {
        printf("Quitting, bye then.\n");
        core->state = CORE_STATE_STOPPED;
        return OPTION_SOME;
    }

    /* Help */
    if(keyword_match_front(&kw_help, msg) == OPTION_SOME) {
        printf("Help? Blah! Help yourself dude :p\n");
        return OPTION_SOME;
    }

    return OPTION_NONE;
}

static option_t system_user_input_examine_room(player_t* player, room_t* current_room, const char* msg) {
    /* Look */
    if(keyword_match_front(&kw_look, msg) == OPTION_SOME) {
        printf("You are in %s and see %d NPCs.\n", current_room->name, current_room->npcs_count);
        return OPTION_SOME;
    }
    return OPTION_NONE;
}

static option_t system_user_input_manipulate_items(player_t* player, room_t* current_room, const char* msg) {
return OPTION_NONE;
}

static option_t system_user_input_player_go(player_t* player, room_t* current_room, const char* msg) {
    word_iterator_start(&word_split_iterator, msg);

    if(word_iterator_has_next(&word_split_iterator) == OPTION_SOME) {
        const char* word = word_split_iterator.next_word;
        const char* remaining = word_split_iterator.next_word_src;
        const int word_index = word_split_iterator.next_word_index;

        /* Check if first word matches keword. Checking index is redundant. */
        if(word_index == 0 && keyword_match(&kw_go, word) == OPTION_SOME) {
            if(current_room->adjecent_rooms_count == 0) {
                printf("You can't go anywhere from here.\n");
                return OPTION_SOME;
            }

            /* Single keyword */
            if(strlen(remaining) == 0) {
                /* Iterate over list of adjecent rooms */
                printf("You can go to:\n");

                for(int i=0; i<current_room->adjecent_rooms_count; i++) {
                    room_t* adjecent_room = current_room->adjecent_rooms[i];
                    printf(" * %s\n", adjecent_room->name);
                }

                // TODO
                // room_iter_t adjecent_room_iter = room_get_adjecent_room_iter(current_room);
                // room_t* adjecent_room = NULL;
                // iterator_foreach(&adjecent_room, &adjecent_room_iter) {
                //     printf(" * %s\n", adjecent_room->name);
                // }
                return OPTION_SOME;
            }

            /* Keyword with remaining direction */
            for(int i=0; i<current_room->adjecent_rooms_count; i++) {
                room_t* adjecent_room = current_room->adjecent_rooms[i];
                if(string_compare_ignorecase(adjecent_room->name, remaining) == OPTION_SOME) {
                    player_change_room(player, adjecent_room);
                    return OPTION_SOME;
                }
            }
            printf("There is no road leading to %s.\n", remaining);
            return OPTION_SOME;
        }
    }
    return OPTION_NONE;
}

static option_t system_user_input_npc_interaction(player_t* player, room_t* current_room, const char* msg) {
        /* Try interacting with NPCs */
    // room_iter_t room_iter = world_get_room_iter(world);
    // printf("Iterating over %d rooms in the world:\n", room_iter.count);

    // room_t* a_room = NULL;    
    // iterator_foreach(&a_room, &room_iter) {
        // npc_iter_t npc_iter = room_get_npc_iter(current_room);
        // printf(" * iterating over %d NPCs in the room %s:\n", npc_iter.count, current_room->name);
        
        // npc_t* a_npc = NULL;
        // iterator_foreach(&a_npc, &npc_iter) {
        //     printf("   - %d NPC: %s,\n", npc_iter.current, a_npc->name);
        // }
    // }

    return OPTION_NONE;
}

static void system_user_input_on_unknown() {
    printf("What?\n");
}

/* Public interface */
void system_user_input_init() {
    keywords_list_from_delimited_string(&kws_exit, "!exit,!quit,!e,!q,!leave", ",");
    keyword_from_string(&kw_help, "!help");
    keyword_from_string(&kw_look, "look");
    keyword_from_string(&kw_go, "go");
}

void system_user_input_process(core_t* core, player_t* player, const char* msg) {
    debug_printf("Player %s says \'%s\' so...\n", player->name, msg);

    /* General */
    if(system_user_input_general(core, player, msg) == OPTION_SOME) {
        return;
    } 
    
    room_t* current_room = player->current_room;
    if(current_room) {
        /* Examin room */
        if(system_user_input_examine_room(player, current_room, msg) == OPTION_SOME ) {
            return;
        } 
        
        /* Manipulate items */
        if(system_user_input_manipulate_items(player, current_room, msg) == OPTION_SOME ) {
            return;
        } 

        /* Movement */
        if(system_user_input_player_go(player, current_room, msg) == OPTION_SOME ) {
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
