#include <gtest/gtest.h>

extern "C" {
#include "utils.h"
#include "keywords.h"
#include "dialog.h"
#include "responses.h"
}

/* Keyword creation */
TEST(KeywordTest, KeywordInitialization) {
  keyword_t keyword;
  keyword_init(&keyword);

  EXPECT_EQ(keyword.length, 0);
  EXPECT_STREQ(keyword.text, "");
}

TEST(KeywordTest, KeywordCreationFromString) {
  keyword_t keyword;
  result_t creation_result = keyword_from_string(&keyword, "hi there");

  EXPECT_EQ(creation_result, RESULT_OK);
  EXPECT_STREQ(keyword.text, "hi there");
  EXPECT_EQ(keyword.length, (int)(strlen("hi there")));
}


/* Keyword match front */
TEST(KeywordTest, KeywordMatchFrontSimpleWord) {
  keyword_t keyword;
  result_t creation_result = keyword_from_string(&keyword, "go");
  /* Correct */
  {
    option_t kw_match_result_1 = keyword_match_front_ignorecase(&keyword, "go");
    EXPECT_EQ(kw_match_result_1, OPTION_SOME);
  }
  {
    option_t kw_match_result_1 = keyword_match_front_ignorecase(&keyword, "go ");
    EXPECT_EQ(kw_match_result_1, OPTION_SOME);
  }
  {
    option_t kw_match_result_1 = keyword_match_front_ignorecase(&keyword, "go\n");
    EXPECT_EQ(kw_match_result_1, OPTION_SOME);
  }
  {
    /* Leading space passes, but it should be removed by normalizing string. */
    option_t kw_match_result_1 = keyword_match_front_ignorecase(&keyword, " go");
    EXPECT_EQ(kw_match_result_1, OPTION_SOME);
  }

  /* Inorrect */
  {
    option_t kw_match_result_2 = keyword_match_front_ignorecase(&keyword, "good");
    EXPECT_EQ(kw_match_result_2, OPTION_NONE);
  }
}

TEST(KeywordTest, KeywordMatchFrontTwoWords) {
  keyword_t keyword;
  result_t creation_result = keyword_from_string(&keyword, "hi there");

  /* Correct */
  {
    option_t match_front_result1 = keyword_match_front_ignorecase(&keyword, "hi there");
    EXPECT_EQ(match_front_result1, OPTION_SOME);
  }
  {
    option_t match_front_result2 = keyword_match_front_ignorecase(&keyword, "hi there ");
    EXPECT_EQ(match_front_result2, OPTION_SOME);
  }
  {
    option_t match_front_result3 = keyword_match_front_ignorecase(&keyword, "hi there my friend");
    EXPECT_EQ(match_front_result3, OPTION_SOME);
  }
  {
    option_t match_front_result4 = keyword_match_front_ignorecase(&keyword, "Hi there my friend");
    EXPECT_EQ(match_front_result4, OPTION_SOME);
  }
  {
    /* Leading space passes, but it should be removed by normalizing string. */
    option_t match_front_result6 = keyword_match_front_ignorecase(&keyword, " hi there");
    EXPECT_EQ(match_front_result6, OPTION_SOME);
  }
  {
    /* Leading space passes, but it should be removed by normalizing string. */
    option_t match_front_result10 = keyword_match_front_ignorecase(&keyword, " hi there my friend");
    EXPECT_EQ(match_front_result10, OPTION_SOME);
  }

  /* Inorrect */
  {
    option_t match_front_result5 = keyword_match_front_ignorecase(&keyword, "hi");
    EXPECT_EQ(match_front_result5, OPTION_NONE);
  }
  {
    option_t match_front_result5 = keyword_match_front_ignorecase(&keyword, "hi ");
    EXPECT_EQ(match_front_result5, OPTION_NONE);
  }
  {
    option_t match_front_result5 = keyword_match_front_ignorecase(&keyword, "hi t");
    EXPECT_EQ(match_front_result5, OPTION_NONE);
  }
  {
    option_t match_front_result5 = keyword_match_front_ignorecase(&keyword, "hi therx");
    EXPECT_EQ(match_front_result5, OPTION_NONE);
  }
  {
    option_t match_front_result7 = keyword_match_front_ignorecase(&keyword, " hi ther");
    EXPECT_EQ(match_front_result7, OPTION_NONE);
  }
  {
    option_t match_front_result8 = keyword_match_front_ignorecase(&keyword, "hi ther");
    EXPECT_EQ(match_front_result8, OPTION_NONE);
  }
  {
    option_t match_front_result9 = keyword_match_front_ignorecase(&keyword, "hi ther ");
    EXPECT_EQ(match_front_result9, OPTION_NONE);
  }
  {
    option_t match_front_result11 = keyword_match_front_ignorecase(&keyword, "not hi there my friend");
    EXPECT_EQ(match_front_result11, OPTION_NONE);
  }
  {
    option_t match_front_result = keyword_match_front_ignorecase(&keyword, "hi theremy friend blah");
    EXPECT_EQ(match_front_result, OPTION_NONE);
  }
}


/* Keyword match any */
TEST(KeywordTest, KeywordMatchAnySimpleWord) {
  keyword_t keyword;
  result_t creation_result = keyword_from_string(&keyword, "go");
  /* Correct */
  {
    option_t kw_match_result_1 = keyword_match_any_ignorecase(&keyword, "go");
    EXPECT_EQ(kw_match_result_1, OPTION_SOME);
  }
  {
    option_t kw_match_result_1 = keyword_match_any_ignorecase(&keyword, "gO ");
    EXPECT_EQ(kw_match_result_1, OPTION_SOME);
  }
  {
    option_t kw_match_result_1 = keyword_match_any_ignorecase(&keyword, "he Go");
    EXPECT_EQ(kw_match_result_1, OPTION_SOME);
  }
  {
    option_t kw_match_result_1 = keyword_match_any_ignorecase(&keyword, "he GO ");
    EXPECT_EQ(kw_match_result_1, OPTION_SOME);
  }
  {
    /* Leading space passes, but it should be removed by normalizing string. */
    option_t kw_match_result_1 = keyword_match_any_ignorecase(&keyword, " go");
    EXPECT_EQ(kw_match_result_1, OPTION_SOME);
  }

  /* Inorrect */
  {
    option_t kw_match_result_2 = keyword_match_any_ignorecase(&keyword, "good");
    EXPECT_EQ(kw_match_result_2, OPTION_NONE);
  }
  {
    option_t kw_match_result_2 = keyword_match_any_ignorecase(&keyword, "g");
    EXPECT_EQ(kw_match_result_2, OPTION_NONE);
  }
  {
    option_t kw_match_result_2 = keyword_match_any_ignorecase(&keyword, "g o");
    EXPECT_EQ(kw_match_result_2, OPTION_NONE);
  }
  {
    option_t kw_match_result_2 = keyword_match_any_ignorecase(&keyword, "he goes");
    EXPECT_EQ(kw_match_result_2, OPTION_NONE);
  }
  {
    option_t kw_match_result_2 = keyword_match_any_ignorecase(&keyword, "he g");
    EXPECT_EQ(kw_match_result_2, OPTION_NONE);
  }
}

TEST(KeywordTest, KeywordMatchAnyTwoWord) {
  keyword_t keyword;
  result_t creation_result = keyword_from_string(&keyword, "go left");
  /* Correct */
  {
    option_t kw_match_result_1 = keyword_match_any_ignorecase(&keyword, "go left");
    EXPECT_EQ(kw_match_result_1, OPTION_SOME);
  }
  {
    option_t kw_match_result_1 = keyword_match_any_ignorecase(&keyword, "go left then");
    EXPECT_EQ(kw_match_result_1, OPTION_SOME);
  }
  {
    option_t kw_match_result_1 = keyword_match_any_ignorecase(&keyword, "I Go left");
    EXPECT_EQ(kw_match_result_1, OPTION_SOME);
  }
  {
    option_t kw_match_result_1 = keyword_match_any_ignorecase(&keyword, "I go leFt ");
    EXPECT_EQ(kw_match_result_1, OPTION_SOME);
  }
  {
    /* Leading space passes, but it should be removed by normalizing string. */
    option_t kw_match_result_2 = keyword_match_any_ignorecase(&keyword, " go left");
    EXPECT_EQ(kw_match_result_2, OPTION_SOME);
  }

  /* Inorrect */
  {
    option_t kw_match_result_2 = keyword_match_any_ignorecase(&keyword, "go lef");
    EXPECT_EQ(kw_match_result_2, OPTION_NONE);
  }
  {
    option_t kw_match_result_2 = keyword_match_any_ignorecase(&keyword, "gx left");
    EXPECT_EQ(kw_match_result_2, OPTION_NONE);
  }
  {
    option_t kw_match_result_2 = keyword_match_any_ignorecase(&keyword, "goo left");
    EXPECT_EQ(kw_match_result_2, OPTION_NONE);
  }
  {
    option_t kw_match_result_2 = keyword_match_any_ignorecase(&keyword, "I goo left");
    EXPECT_EQ(kw_match_result_2, OPTION_NONE);
  }
}


/* Keywords list creation */
TEST(KeywordsListTest, KeywordsListInit) {
  keywords_list_t keywords_list;
  keywords_list_init(&keywords_list);
  EXPECT_EQ(keywords_list.count, 0);
  EXPECT_EQ(keywords_list.keywords[0].length, 0);
  EXPECT_EQ(keywords_list.keywords[0].text[0], '\0');
}

TEST(KeywordsListTest, KeywordsListCreationFromArray) {
  keywords_list_t keywords_list;
  result_t creation_result = keywords_list_from_array(&keywords_list, (const char*[]){"go up", "left", "right"}, 3);
  EXPECT_EQ(creation_result, RESULT_OK);
  EXPECT_EQ(keywords_list.count, 3);

  EXPECT_STREQ(keywords_list.keywords[0].text, "go up");
  EXPECT_EQ(keywords_list.keywords[0].length, 5);
  EXPECT_STREQ(keywords_list.keywords[1].text, "left");
  EXPECT_EQ(keywords_list.keywords[1].length, 4);
  EXPECT_STREQ(keywords_list.keywords[2].text, "right");
  EXPECT_EQ(keywords_list.keywords[2].length, 5);
}

TEST(KeywordsListTest, KeywordsListCreationFromDelimitedString) {
  {
    keywords_list_t keywords_list;
    result_t creation_result = keywords_list_from_delimited_string(&keywords_list, "go up,left,right", ",");
    EXPECT_EQ(creation_result, RESULT_OK);
    EXPECT_EQ(keywords_list.count, 3);

    EXPECT_STREQ(keywords_list.keywords[0].text, "go up");
    EXPECT_EQ(keywords_list.keywords[0].length, 5);
    EXPECT_STREQ(keywords_list.keywords[1].text, "left");
    EXPECT_EQ(keywords_list.keywords[1].length, 4);
    EXPECT_STREQ(keywords_list.keywords[2].text, "right");
    EXPECT_EQ(keywords_list.keywords[2].length, 5);
  }
  {
    keywords_list_t keywords_list;
    result_t creation_result = keywords_list_from_delimited_string(&keywords_list, "hi, hello, greetings", ",");
    EXPECT_EQ(creation_result, RESULT_OK);
    EXPECT_EQ(keywords_list.count, 3);
    EXPECT_STREQ(keywords_list.keywords[0].text, "hi");
    EXPECT_STREQ(keywords_list.keywords[1].text, "hello");
    EXPECT_STREQ(keywords_list.keywords[2].text, "greetings");
  }
}

/* Keywords list match front */


/* Keywords list match any */


/* Dialog cond if */
TEST(DialogsTest, DialogCondIfInit) {
  dialog_cond_if_t dialog_cond_if;
  dialog_cond_if_init(&dialog_cond_if);
  ASSERT_EQ(dialog_cond_if.dialog_stage, DIALOG_STAGE_NONE);
  ASSERT_EQ(dialog_cond_if.keywords.count, 0);
  ASSERT_EQ(dialog_cond_if.keyword_wildcard.type, WILDCARD_TYPE_NONE);
  ASSERT_EQ(dialog_cond_if.keyword_wildcard.type, WILDCARD_TYPE_NONE);
}


/* Dialog cond then */
TEST(DialogsTest, DialogCondThenInit) {
  
}


/* Dialog blocks */
TEST(DialogsTest, DialogBlockInit) {

}

/* Responses */

