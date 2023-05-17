#include <gtest/gtest.h>

extern "C" {
#include "utils.h"
}

TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("worsld", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

TEST(UtilsTest, StringEqualsIgnorecase) {
  /* Correct */
  {
    option_t strings_equal_ignore_different_charcase = string_equals_ignorecase("Hello", "hello");
    EXPECT_EQ(strings_equal_ignore_different_charcase, OPTION_SOME);
  }
  {
    option_t strings_equal = string_equals_ignorecase("Hello", "Hello");
    EXPECT_EQ(strings_equal, OPTION_SOME);
  }

  /* Incorrect */
  {
  option_t string_not_equal_leading_second_space = string_equals_ignorecase("Hallo", " hello");
  EXPECT_EQ(string_not_equal_leading_second_space, OPTION_NONE);
  }
  {
  option_t string_not_equal_following_second_space = string_equals_ignorecase("Hello", "hello ");
  EXPECT_EQ(string_not_equal_following_second_space, OPTION_NONE);
  }
  {
    option_t string_not_equal_leading_first_space = string_equals_ignorecase(" Hello", "hello");
    EXPECT_EQ(string_not_equal_leading_first_space, OPTION_NONE);
  }
  {
  option_t string_not_equal_following_first_space = string_equals_ignorecase("Hello ", "hello");
  EXPECT_EQ(string_not_equal_following_first_space, OPTION_NONE);
  }
  {
    option_t string_not_equal_different_spaces = string_equals_ignorecase("Hello ", " hello");
    EXPECT_EQ(string_not_equal_different_spaces, OPTION_NONE);
  }
}

TEST(UtilsTest, StringSubEqualsIgnorecase) {
  /* Correct */
  {
    option_t string_equal = string_sub_equals_ignorecase("Helldsasdsadlo", 3, "hello", 3);
    EXPECT_EQ(string_equal, OPTION_SOME);
  }

  /* Incorrect */
  {
  option_t string_not_equal_bad_char = string_sub_equals_ignorecase("Htllo", 4, "hello", 4);
  EXPECT_EQ(string_not_equal_bad_char, OPTION_NONE);
  }
  {
  option_t string_not_equal_uneven_length = string_sub_equals_ignorecase("Hallo", 3, "hello", 4);
  EXPECT_EQ(string_not_equal_uneven_length, OPTION_NONE);
  }
  {
  option_t failed_substring_loonger_than_string = string_sub_equals_ignorecase("hello", 8, "hello", 8);
  EXPECT_EQ(failed_substring_loonger_than_string, OPTION_NONE);
  }
}

TEST(UtilsTest, StringMatchIgnorecase) {
  /* Correct */
  {
    option_t string_equal4 = string_match_ignorecase("Hello world", "Hello world");
    EXPECT_EQ(string_equal4, OPTION_SOME);
  }
  {
    option_t string_equal4 = string_match_ignorecase("Hello world", "Hello worl");
    EXPECT_EQ(string_equal4, OPTION_SOME);
  }
  {
    option_t string_equal = string_match_ignorecase("Hello world", "world");
    EXPECT_EQ(string_equal, OPTION_SOME);
  }
  {
    option_t string_equal2 = string_match_ignorecase("Hello world", "lo wor");
    EXPECT_EQ(string_equal2, OPTION_SOME);
  }
  {
    option_t string_equal3 = string_match_ignorecase("Hello world", "Hello ");
    EXPECT_EQ(string_equal3, OPTION_SOME);
  }
  {
    option_t string_equal3 = string_match_ignorecase("Hello world", "Hello");
    EXPECT_EQ(string_equal3, OPTION_SOME);
  }
  {
    option_t string_equal4 = string_match_ignorecase("Hello world", "Hell");
    EXPECT_EQ(string_equal4, OPTION_SOME);
  }
  {
    option_t string_equal4 = string_match_ignorecase("Hello world", "o world");
    EXPECT_EQ(string_equal4, OPTION_SOME);
  }
  {
    option_t string_equal4 = string_match_ignorecase("Hello world", " world");
    EXPECT_EQ(string_equal4, OPTION_SOME);
  }
  {
    option_t string_equal4 = string_match_ignorecase("Hello world", "world");
    EXPECT_EQ(string_equal4, OPTION_SOME);
  }
  {
    option_t string_equal4 = string_match_ignorecase("Hello world", "");
    EXPECT_EQ(string_equal4, OPTION_SOME);
  }

  /* Incorrect */
  {
  option_t string_not_equal_bad_char = string_match_ignorecase("Hello world", "Hello worlx");
  EXPECT_EQ(string_not_equal_bad_char, OPTION_NONE);
  }
  {
  option_t string_not_equal_bad_char = string_match_ignorecase("", "Hello worldd");
  EXPECT_EQ(string_not_equal_bad_char, OPTION_NONE);
  }
  {
  option_t string_not_equal_bad_char = string_match_ignorecase("Hello world", "Hello world1");
  EXPECT_EQ(string_not_equal_bad_char, OPTION_NONE);
  }
}


TEST(UtilsTest, StringMatchFrontIgnorecase) {
  /* Correct */
  {
    option_t string_equal4 = string_match_front_ignorecase("Hello world", "Hello world");
    EXPECT_EQ(string_equal4, OPTION_SOME);
  }
  {
    option_t string_equal4 = string_match_front_ignorecase("Hello world", "Hello worl");
    EXPECT_EQ(string_equal4, OPTION_SOME);
  }
  {
    option_t string_equal4 = string_match_front_ignorecase("Hello world", "Hello ");
    EXPECT_EQ(string_equal4, OPTION_SOME);
  }
  {
    option_t string_equal4 = string_match_front_ignorecase("Hello world", "Hello");
    EXPECT_EQ(string_equal4, OPTION_SOME);
  }
  {
    option_t string_equal4 = string_match_front_ignorecase("Hello world", "Hell");
    EXPECT_EQ(string_equal4, OPTION_SOME);
  }
  {
    option_t string_equal4 = string_match_front_ignorecase("Hello world", "H");
    EXPECT_EQ(string_equal4, OPTION_SOME);
  }
  {
    option_t string_equal4 = string_match_front_ignorecase("Hello world", "");
    EXPECT_EQ(string_equal4, OPTION_SOME);
  }

  /* Incorrect */
  {
  option_t string_not_matched = string_match_front_ignorecase("Hello world", "ello world");
  EXPECT_EQ(string_not_matched, OPTION_NONE);
  }
  {
  option_t string_not_matched = string_match_front_ignorecase("Hello world", "d");
  EXPECT_EQ(string_not_matched, OPTION_NONE);
  }
  {
  option_t string_not_matched = string_match_front_ignorecase("Hello world", " Hello");
  EXPECT_EQ(string_not_matched, OPTION_NONE);
  }
  {
  option_t string_not_matched = string_match_front_ignorecase("Hello world", "world");
  EXPECT_EQ(string_not_matched, OPTION_NONE);
  }
  {
  option_t string_not_matched = string_match_front_ignorecase("Hello world", "Hello worlds");
  EXPECT_EQ(string_not_matched, OPTION_NONE);
  }
}
