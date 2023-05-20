#include <gtest/gtest.h>

extern "C" {
#include "utils.h"
#include "keywords.h"
#include "system_dialogs.h"
#include "dialog.h"
#include "responses.h"
#include "player.h"
#include "npc.h"
}

#define MSG_BUFFER_SIZE 128

/* Keyword creation */
TEST(SystemDialogTest, CondIfMatch) {
  char msg_buffer[MSG_BUFFER_SIZE];
  memset(msg_buffer, '\0', MSG_BUFFER_SIZE);
  const char* msg_content = "hi bim";
  memcpy(msg_buffer, msg_content, strlen(msg_content) * sizeof(char));

  /* Player */  
  player_t a_player;
  memset(&a_player, 0, sizeof(player_t));
  player_set_name(&a_player, "Zbignief");

  /* NPC with default hi-bye dialog */
  npc_t some_npc;
  npc_init(&some_npc);
  npc_set_name(&some_npc, "Bim");

  ASSERT_EQ(some_npc.dialog_blocks_count, 2);
  ASSERT_EQ(some_npc.dialog_stage, 0);

  {
    const dialog_cond_if_t* cond_if_hi = &some_npc.dialog_blocks[0].cond_if;
    ASSERT_EQ(cond_if_hi->dialog_stage, cond_if_hi->dialog_stage);
    ASSERT_GT(cond_if_hi->keywords.count, 0);
    ASSERT_EQ(cond_if_hi->keyword_wildcard.type, WILDCARD_TYPE_NPC_NAME);
    ASSERT_EQ(system_dialog_match_any_npc_name(some_npc.name, msg_buffer), OPTION_SOME);
    
    option_t match_result = system_dialog_match_cond_if(cond_if_hi, &some_npc, &a_player, msg_buffer);
    ASSERT_EQ(match_result, OPTION_SOME);
  }
  {
    const dialog_cond_if_t* cond_if_bye = &some_npc.dialog_blocks[1].cond_if;
    option_t match_result = system_dialog_match_cond_if(cond_if_bye, &some_npc, &a_player, msg_buffer);
    ASSERT_EQ(match_result, OPTION_NONE);
  }
}

TEST(SystemDialogTest, CondIfMatchLonger) {
  int errors_count = 0;
  npc_t test_npc_3;
  npc_init(&test_npc_3);
  test_npc_3.id = 3;
  if(npc_set_name(&test_npc_3, "Bim") == RESULT_ERROR) {
      error_printf("Cannot assign name to the npc.\n");
      ++errors_count;
  }
  npc_t* npc_bim = &test_npc_3;


  /* Add some more dialogs to NPC */
  {
    /* Next dialog block */
    {
      dialog_block_t dialog_block;
      dialog_block_init(&dialog_block, DIALOG_TYPE_REGULAR);
      dialog_cond_if_t* cond_if = &dialog_block.cond_if;
      
      /* From (if) stage 1 */
      cond_if->dialog_stage = 1;

      /* With (if) keywords */
      if(keywords_list_from_delimited_string(&cond_if->keywords, "how are you,whatsup", ",") != RESULT_OK) {
          error_printf("Couldnt parse keywords.\n");
          ++errors_count;
      }

      /* To (then) stage 2 */
      dialog_block.cond_then.next_dialog_stage = 2;
      response_from_string(&dialog_block.cond_then.response, "Nuffin special. And you?", BOOL_FALSE);

      /* Append dialog block */
      if(npc_append_dialog_block(npc_bim, &dialog_block) != RESULT_OK) {
          /* BTW appending leaves variable invalid */
          error_printf("Cannot add dialog block to NPC.\n");
          ++errors_count;
      }
    }

    /* Next dialog block */
    {
      dialog_block_t dialog_block;
      dialog_block_init(&dialog_block, DIALOG_TYPE_REGULAR);
      dialog_cond_if_t* cond_if = &dialog_block.cond_if;
      
      /* From (if) stage 1 */
      cond_if->dialog_stage = 2;

      /* With (if) keywords */
      if(keywords_list_from_delimited_string(&cond_if->keywords, "good,ok,nothing special", ",") != RESULT_OK) {
          error_printf("Couldnt parse keywords.\n");
          ++errors_count;
      }

      /* To (then) stage 2 */
      dialog_block.cond_then.next_dialog_stage = 1;
      response_from_string(&dialog_block.cond_then.response, "Fine then.", BOOL_FALSE);

      /* Append dialog block */
      if(npc_append_dialog_block(npc_bim, &dialog_block) != RESULT_OK) {
          /* BTW appending leaves variable invalid */
          error_printf("Cannot add dialog block to NPC.\n");
          ++errors_count;
      }
    }

    /* Next dialog block */
    {
      dialog_block_t dialog_block;
      dialog_block_init(&dialog_block, DIALOG_TYPE_REGULAR);
      dialog_cond_if_t* cond_if = &dialog_block.cond_if;
      
      /* From (if) stage 1 */
      cond_if->dialog_stage = 2;

      /* With (if) keywords */
      if(keywords_list_from_delimited_string(&cond_if->keywords, "bad, not really, not good", ",") != RESULT_OK) {
          error_printf("Couldnt parse keywords.\n");
          ++errors_count;
      }

      /* To (then) stage 2 */
      dialog_block.cond_then.next_dialog_stage = 1;
      response_from_string(&dialog_block.cond_then.response, "Ohh bad.", BOOL_FALSE);

      /* Append dialog block */
      if(npc_append_dialog_block(npc_bim, &dialog_block) != RESULT_OK) {
          /* BTW appending leaves variable invalid */
          error_printf("Cannot add dialog block to NPC.\n");
          ++errors_count;
      }
    }
  }
  ASSERT_EQ(errors_count, 0);

  /* Phrase */
  char msg_buffer[MSG_BUFFER_SIZE];
  memset(msg_buffer, '\0', MSG_BUFFER_SIZE);
  const char* msg_content = "hello bim";
  memcpy(msg_buffer, msg_content, strlen(msg_content) * sizeof(char));

  
  /* Player */  
  player_t a_player;
  memset(&a_player, 0, sizeof(player_t));
  player_set_name(&a_player, "Zbignief");

  dialog_cond_if_t* cond_if_hello = &npc_bim->dialog_blocks[0].cond_if;
  ASSERT_EQ(cond_if_hello->dialog_stage, 0);
  ASSERT_EQ(cond_if_hello->keyword_wildcard.type, WILDCARD_TYPE_NPC_NAME);
  ASSERT_EQ(cond_if_hello->keywords.count, 3);
  
  // for(int i=0; i<cond_if_hello->keywords.count; ++i) {
  //   printf("%d: %s\n", i, cond_if_hello->keywords.keywords[i].text);
  // }

  option_t match_reesult = keywords_list_match_any_ignorecase(&cond_if_hello->keywords, msg_content);
  ASSERT_EQ(match_reesult, OPTION_SOME);

    // if(keywords_list_match_any_ignorecase(&cond_if->keywords, msg) != OPTION_SOME) {
    //     return OPTION_NONE;
    // }

  option_t result = system_dialog_match_cond_if(cond_if_hello, npc_bim, &a_player, msg_buffer);
  ASSERT_EQ(result, OPTION_SOME);
}