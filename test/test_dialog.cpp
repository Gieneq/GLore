#include <gtest/gtest.h>

extern "C" {
#include "utils.h"
// #include "dialog.h"
#include "keywords.h"
// #include "responses.h"
}


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

TEST(KeywordTest, KeywordMatchFrontSimpleWord) {
  keyword_t keyword;
  result_t creation_result = keyword_from_string(&keyword, "go");
  {
    option_t match_front_result = keyword_match_front_ignorecase(&keyword, "go");
    EXPECT_EQ(match_front_result, OPTION_SOME);
  }

  {
    option_t match_front_result = keyword_match_front_ignorecase(&keyword, "good");
    EXPECT_EQ(match_front_result, OPTION_NONE);
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
    option_t match_front_result2 = keyword_match_front_ignorecase(&keyword, "hi there ");//
    EXPECT_EQ(match_front_result2, OPTION_SOME);
  }
  {
    option_t match_front_result3 = keyword_match_front_ignorecase(&keyword, "hi there my friend");//
    EXPECT_EQ(match_front_result3, OPTION_SOME);
  }
  {
    option_t match_front_result4 = keyword_match_front_ignorecase(&keyword, "Hi there my friend");//
    EXPECT_EQ(match_front_result4, OPTION_SOME);
  }

  /* Inorrect */
  {
    option_t match_front_result5 = keyword_match_front_ignorecase(&keyword, "hi therx");
    EXPECT_EQ(match_front_result5, OPTION_NONE);
  }
  {
    option_t match_front_result6 = keyword_match_front_ignorecase(&keyword, " hi there");
    EXPECT_EQ(match_front_result6, OPTION_NONE);
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
    option_t match_front_result10 = keyword_match_front_ignorecase(&keyword, " hi there my friend");
    EXPECT_EQ(match_front_result10, OPTION_NONE);
  }
  {
    option_t match_front_result11 = keyword_match_front_ignorecase(&keyword, "not hi there my friend");
    EXPECT_EQ(match_front_result11, OPTION_NONE);
  }
  // {
  //   option_t match_front_result = keyword_match_front_ignorecase(&keyword, "hi theremy friend blah");
  //   EXPECT_EQ(match_front_result, OPTION_NONE);
  // }
}