#include "core.h"
#include <stdio.h>
#include <string.h>
#include "loader.h"
#include "loader_testyard.h"
#include "world.h"
#include "system_player_communication.h"


result_t core_create(core_t *core) {
    if (core == NULL) {
        return RESULT_ERROR;
    }
    core->state = CORE_STATE_RUNNING;

    if(world_init(&(core->world)) != RESULT_OK) {
        return RESULT_ERROR;
    }

    system_player_communication_init();

    return RESULT_OK;
}

result_t core_delete(core_t* core) {   
    if (core == NULL) {
        return RESULT_ERROR;
    }
    free(core); //waj?
    return RESULT_OK;
}

result_t core_populate(core_t* core, database_t* database) {
    player_set_name(&core->world.player, "Zbignief");
#if CONFIG_TEST_DATABASE == 1
    if(loader_testyard_populate(&(core->world), database) != RESULT_OK) {
        printf("Problem with test loader.\n");
        return RESULT_ERROR;
    }
#else
    
    // if(loader_parse(&(core->world), database) != RESULT_OK) {
    //     return RESULT_ERROR;
    // }

    // printf("Player name: %s\n", core->world.player.name);
    // printf("Player health: %d\n", core->world.player.stats.health);
    // printf("Player exp: %d\n", core->world.player.stats.experience);

    // printf("Quests count: %llu\n", database_get_quests_count(database));

    // quest_data_t quest_data;
    // database_get_quests_data_by_id(database, 1, &quest_data);
    // quest_data_printf(&quest_data);

#endif

    /* Set starting room for player */
    player_t* player = &core->world.player;
    room_t* selected_room = &core->world.rooms[0];

    if(player_change_room(player, selected_room) != RESULT_OK) {
        printf("Player cannot change room.");
        return RESULT_ERROR;
    }
    
    selected_room = &core->world.rooms[1];
    if(player_change_room(player, selected_room) != RESULT_OK) {
        printf("Player cannot change room.");
        return RESULT_ERROR;
    }

    return RESULT_OK;
}

void core_input(core_t* core) {
    char c = getchar();
    printf("%c", c);
    if (c == '\n' || c == '\r' || c == '\0') {
        // _core_process_cmd(core);
        system_player_communication_process(&core, &core->world.player, core->arg_buffer);
        memset(core->arg_buffer, '\0', CORE_ARG_BUFFER_SIZE);
        core->arg_buffer_index = 0;
    } else {
        if (core->arg_buffer_index < CORE_ARG_MAX_LENGTH) {
            core->arg_buffer[core->arg_buffer_index] = c;
            core->arg_buffer_index++;
        }
    }
}

void core_loop(core_t* core) {

    // world_t* world = &core->world;
    // player_t* player = &world->player;

    // core->state = CORE_STATE_STOPPED;
}


core_state_t core_get_state(core_t* c) {
    return c->state;
}

