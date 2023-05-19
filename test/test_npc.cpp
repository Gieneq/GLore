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
  const int initial_bloks_count = 2;
  ASSERT_EQ(some_npc.dialog_blocks_count, initial_bloks_count);
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