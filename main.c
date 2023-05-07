#include <stdlib.h>
#include <stdio.h>
#include "core.h"
#include "database.h"


static database_t database;
static core_t core;

int main(int argc, char *argv[]) {
    printf("Initializing\n");

    if(core_create(&core) != RESULT_OK) {
        printf("Failed to create core\n");
        return 1;
    }

    database_init(&database);

    if(core_populate(&core, &database) != RESULT_OK) {
        printf("Failed to populate world\n");
        return 1;
    }

    printf("Starting!\n");
    // return 0;

    while(core_get_state(&core) == CORE_STATE_RUNNING) {
        core_input(&core);
        core_loop(&core);
    }

    //save database
    // core_delete(&core);
    printf("Finished\n");
    return 0;
}