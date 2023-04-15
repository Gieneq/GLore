#include <stdlib.h>
#include <stdio.h>
#include "core.h"

static core_t core;
int main(int argc, char *argv[]) {
    printf("Launching\n");

    core_create(&core);

    while(core_get_state(core) == CORE_STATE_RUNNING) {
        core_input(core);
        core_loop(core);
    }


    core_delete(core);
    printf("Finished\n");
    return 0;
}