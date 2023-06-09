#include "core.h"
#include <stdio.h>
#include <string.h>
#include "loader.h"
#include "system_user_input.h"
#include "system_player.h"


static void core_propcess_user_input(core_t* core) {
    string_normalize(core->arg_buffer);
    system_user_input_process(core, &core->world.player, core->arg_buffer);
    memset(core->arg_buffer, '\0', CORE_ARG_BUFFER_SIZE);
    core->arg_buffer_index = 0;
}

result_t core_create(core_t *core) {
    if (core == NULL) {
        return RESULT_ERROR;
    }
    core->state = CORE_STATE_RUNNING;

    item_database_init(&core->item_database);

    if(world_init(&(core->world)) != RESULT_OK) {
        return RESULT_ERROR;
    }

    system_user_input_init();
    return RESULT_OK;
}

result_t core_delete(core_t* core) {
    if (core == NULL) {
        return RESULT_ERROR;
    }
    free(core); //waj?
    return RESULT_OK;
}

result_t core_populate(core_t* core) {
    player_t* player = &core->world.player;
    world_t* world = &core->world;

    /* Load items database */
    if(loader_load_items_database(&core->item_database) != RESULT_OK) {
        error_printf("Items database not loaded.\n");
        return RESULT_ERROR;
    }
    item_database_print_all(&core->item_database);

    /* Load world data */
    if(loader_load_world(world) != RESULT_OK) {
        error_printf("World not loaded.\n");
        return RESULT_ERROR;
    }

    /* Load player data */
    if(loader_load_player(world, &core->item_database, player) != RESULT_OK) {
        error_printf("Player not loaded.\n");
        return RESULT_ERROR;
    }

    /* Load quests */
    if(loader_load_quests(world, player) != RESULT_OK) {
        error_printf("Quests not loaded.\n");
        return RESULT_ERROR;
    }

#if DEBUG == 1
    world_print_all(world);
#endif

    return RESULT_OK;
}

option_t core_test_code(core_t* core) {
    debug_printf("Test code.\n");

    {
        memset(core->arg_buffer, '\0', CORE_ARG_BUFFER_SIZE);
        const char* msg_ = "look";
        printf("_%s\n", msg_);
        strcpy(core->arg_buffer, msg_);
        core_propcess_user_input(core);
    }
    {
        memset(core->arg_buffer, '\0', CORE_ARG_BUFFER_SIZE);
        const char* msg_ = "hi woodworker bil";
        printf("_%s\n", msg_);
        strcpy(core->arg_buffer, msg_);
        core_propcess_user_input(core);
    }
    {
        memset(core->arg_buffer, '\0', CORE_ARG_BUFFER_SIZE);
        const char* msg_ = "work";
        printf("_%s\n", msg_);
        strcpy(core->arg_buffer, msg_);
        core_propcess_user_input(core);
    }
    {
        memset(core->arg_buffer, '\0', CORE_ARG_BUFFER_SIZE);
        const char* msg_ = "help";
        printf("_%s\n", msg_);
        strcpy(core->arg_buffer, msg_);
        core_propcess_user_input(core);
    }
    {
        memset(core->arg_buffer, '\0', CORE_ARG_BUFFER_SIZE);
        const char* msg_ = "yes";
        printf("_%s\n", msg_);
        strcpy(core->arg_buffer, msg_);
        core_propcess_user_input(core);
    }
    {
        memset(core->arg_buffer, '\0', CORE_ARG_BUFFER_SIZE);
        const char* msg_ = "ql";
        printf("_%s\n", msg_);
        strcpy(core->arg_buffer, msg_);
        core_propcess_user_input(core);
    }
    {
        memset(core->arg_buffer, '\0', CORE_ARG_BUFFER_SIZE);
        const char* msg_ = "work";
        printf("_%s\n", msg_);
        strcpy(core->arg_buffer, msg_);
        core_propcess_user_input(core);
    }

#if DEBUG == 1
    {
        memset(core->arg_buffer, '\0', CORE_ARG_BUFFER_SIZE);
        const char* msg_ = "help";
        strcpy(core->arg_buffer, msg_);
        core_propcess_user_input(core);
    }
    {
        memset(core->arg_buffer, '\0', CORE_ARG_BUFFER_SIZE);
        const char* msg_ = "yes";
        strcpy(core->arg_buffer, msg_);
        core_propcess_user_input(core);
    }
    {
        memset(core->arg_buffer, '\0', CORE_ARG_BUFFER_SIZE);
        const char* msg_ = "ql";
        strcpy(core->arg_buffer, msg_);
        core_propcess_user_input(core);
    }

    return OPTION_SOME;
#else
    return OPTION_NONE;
#endif
}

void core_input(core_t* core) {
    char c = getchar();
    debug_printf("%c", c);
    if (c == '\n' || c == '\r' || c == '\0') {
        core_propcess_user_input(core);
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

