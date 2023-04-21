#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "world.h"
#include "entity.h"

struct world_obj {

};

result_t world_create(world_t * world) {
    *world = malloc(sizeof(struct world_obj));
    if(*world == NULL) {
        return RESULT_ERROR;
    }
    return RESULT_OK;
}

result_t world_delete(world_t world) {
    if(world == NULL) {
        return RESULT_ERROR;
    }
    free(world);
    return RESULT_OK;
}