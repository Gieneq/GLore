#include <gtest/gtest.h>

extern "C" {
#include "utils.h"
#include "npc.h"
#include <string.h>
}


TEST(NPCTest, NPCCreation)
{
  npc_t some_npc;
  npc_init(&some_npc);
  some_npc.id = 1;
  ASSERT_EQ(npc_set_name(&some_npc, "Ivan"), RESULT_OK);
  ASSERT_EQ(some_npc.dialog_blocks_count, 0);
  
}

//todo - first test dialog blocks

// TEST(NPCTest, NPCMatchUserInput)
// {
//     npc_t some_npc;
//     npc_init(&some_npc);
//     some_npc.id = 1;
//     ASSERT_EQ(npc_set_name(&some_npc, "Ivan"), RESULT_OK);

//     npc_match_user_input(&some_npc, "Ivan");
// }