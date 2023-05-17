#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("worsld", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

extern "C" {
  // extern option_t string_compare_ignorecase(const char *str1, const char *str2);
#include "utils.h"
}

TEST(UtilsTest, StringCompare) {
  /* Correct */
  {
    option_t result_correct = string_compare_ignorecase("Hello", "hello");
    EXPECT_EQ(result_correct, OPTION_SOME);
  }

  /* Incorrect */
  {
  option_t result_incorrect = string_compare_ignorecase("Hallo", " hello");
  EXPECT_EQ(result_incorrect, OPTION_NONE);
  }
  {
  option_t result_incorrect = string_compare_ignorecase("Hello", "hello ");
  EXPECT_EQ(result_incorrect, OPTION_NONE);
  }
  {
    option_t result_incorrect = string_compare_ignorecase(" Hello", "hello");
    EXPECT_EQ(result_incorrect, OPTION_NONE);
  }
  {
  option_t result_incorrect = string_compare_ignorecase("Hello ", "hello");
  EXPECT_EQ(result_incorrect, OPTION_NONE);
  }
  {
    option_t result_incorrect = string_compare_ignorecase("Hello ", " hello");
    EXPECT_EQ(result_incorrect, OPTION_NONE);
  }
}