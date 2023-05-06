#include "core.h"
#include <stdio.h>
#include <string.h>
#include "loader.h"
#include "loader_testyard.h"
#include "world.h"


static void _core_process_cmd(core_t* core) {
    if (strcmp(core->arg_buffer, "quit") == 0) {
        core->state = CORE_STATE_STOPPED;
    } 
    
    else if (strcmp(core->arg_buffer, "help") == 0) {
        printf("Commands:\n");
        printf("quit - quit the game\n");
        printf("help - show this help\n");
    } 

    else {
        printf("Unknown command: %s\n", core->arg_buffer);
    }

    memset(core->arg_buffer, '\0', CORE_ARG_BUFFER_SIZE);
    core->arg_buffer_index = 0;
}


result_t core_create(core_t *core) {
    if (core == NULL) {
        return RESULT_ERROR;
    }
    core->state = CORE_STATE_RUNNING;

    if(world_init(&(core->world)) != RESULT_OK) {
        return RESULT_ERROR;
    }

    return RESULT_OK;
}

result_t core_delete(core_t* core) {   
    if (core == NULL) {
        return RESULT_ERROR;
    }
    free(core);
    return RESULT_OK;
}

result_t core_populate(core_t* core, database_t* database) {
#if CONFIG_TEST_DATABASE == 1
    if(loader_testyard_populate(&(core->world), database) != RESULT_OK) {
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
    return RESULT_OK;
}

void core_input(core_t* core) {
    char c = getchar();
    // printf("%c", c);
    if (c == '\n' || c == '\r' || c == '\0') {
        _core_process_cmd(core);
    } else {
        if (core->arg_buffer_index < CORE_ARG_MAX_LENGTH) {
            core->arg_buffer[core->arg_buffer_index] = c;
            core->arg_buffer_index++;
        }
    }
}

void core_loop(core_t* core) {
    
}


core_state_t core_get_state(core_t* c) {
    return c->state;
}

