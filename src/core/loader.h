#pragma once
#include "utils.h"
#include "world.h"
#include "database.h"

/*
 * Function to get world, npc, player, items and objects dagta from data files.
*/
result_t loader_parse(world_t* world, database_t* database);