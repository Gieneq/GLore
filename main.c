#include <stdlib.h>
#include <stdio.h>
#include "core.h"
#include "database.h"
#include "utils.h"
#include <windows.h>
static database_t database;
static core_t core;

int main(int argc, char *argv[]) {
    debug_printf("Initializing\n");
    fancy_cmd_init();

    if(core_create(&core) != RESULT_OK) {
        error_printf("Failed to create core\n");
        return 1;
    }

    database_init(&database);

    if(core_populate(&core, &database) != RESULT_OK) {
        error_printf("Failed to populate world\n");
        return 1;
    }

    narrator_printf("Welcome in GLore! You are in text world in which \nyou can navigate by writing words. Keywords are fine. \nIf you need further information type \'help\'. Good luck!\n");
    narrator_printf("---------------------------------------------------------\n");

    if(core_test_code(&core) == OPTION_SOME) {
        return 0;
    }

    while(core_get_state(&core) == CORE_STATE_RUNNING) {
        core_input(&core);
        core_loop(&core);
    }

    //save database
    // core_delete(&core);
    debug_printf("Finished\n");
    return 0;
}