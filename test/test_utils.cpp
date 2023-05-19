#include <gtest/gtest.h>

extern "C" {
#include "utils.h"
#include <string.h>
}


/* String tools */
TEST(UtilsTest, StringStrip) {
  {
    const char* sample_text = "hello";
    const int sample_text_len = strlen(sample_text);
    size_t sample_text_size = sizeof(char)*sample_text_len;

    char text_buffer[32];
    memset(text_buffer, '\0', sizeof(text_buffer));
    memcpy(text_buffer, sample_text, sample_text_size);

    string_strip(text_buffer);
    EXPECT_STREQ(text_buffer, "hello");
  }

  /* Trailing spaces */
  {
    const char* sample_text = "hello ";
    const int sample_text_len = strlen(sample_text);
    size_t sample_text_size = sizeof(char)*sample_text_len;

    char text_buffer[32];
    memset(text_buffer, '\0', sizeof(text_buffer));
    memcpy(text_buffer, sample_text, sample_text_size);

    string_strip(text_buffer);
    EXPECT_STREQ(text_buffer, "hello");
  }
  {
    const char* sample_text = "hello   ";
    const int sample_text_len = strlen(sample_text);
    size_t sample_text_size = sizeof(char)*sample_text_len;

    char text_buffer[32];
    memset(text_buffer, '\0', sizeof(text_buffer));
    memcpy(text_buffer, sample_text, sample_text_size);

    string_strip(text_buffer);
    EXPECT_STREQ(text_buffer, "hello");
  }
  {
    const char* sample_text = "hello    ";
    const int sample_text_len = strlen(sample_text);
    size_t sample_text_size = sizeof(char)*sample_text_len;

    char text_buffer[32];
    memset(text_buffer, '\0', sizeof(text_buffer));
    memcpy(text_buffer, sample_text, sample_text_size);

    string_strip(text_buffer);
    EXPECT_STREQ(text_buffer, "hello");
    string_strip(text_buffer);
    EXPECT_STREQ(text_buffer, "hello");
  }
  {
    const char* sample_text = "hel lo ";
    const int sample_text_len = strlen(sample_text);
    size_t sample_text_size = sizeof(char)*sample_text_len;

    char text_buffer[32];
    memset(text_buffer, '\0', sizeof(text_buffer));
    memcpy(text_buffer, sample_text, sample_text_size);

    string_strip(text_buffer);
    EXPECT_STREQ(text_buffer, "hel lo");
  }

  /* Leading spaces */
  {
    const char* sample_text = " hello";
    const int sample_text_len = strlen(sample_text);
    size_t sample_text_size = sizeof(char)*sample_text_len;

    char text_buffer[32];
    memset(text_buffer, '\0', sizeof(text_buffer));
    memcpy(text_buffer, sample_text, sample_text_size);

    string_strip(text_buffer);
    EXPECT_STREQ(text_buffer, "hello");
  }
  {
    const char* sample_text = "     hello";
    const int sample_text_len = strlen(sample_text);
    size_t sample_text_size = sizeof(char)*sample_text_len;

    char text_buffer[32];
    memset(text_buffer, '\0', sizeof(text_buffer));
    memcpy(text_buffer, sample_text, sample_text_size);

    string_strip(text_buffer);
    EXPECT_STREQ(text_buffer, "hello");
  }
  {
    const char* sample_text = "    hel lo";
    const int sample_text_len = strlen(sample_text);
    size_t sample_text_size = sizeof(char)*sample_text_len;

    char text_buffer[32];
    memset(text_buffer, '\0', sizeof(text_buffer));
    memcpy(text_buffer, sample_text, sample_text_size);

    string_strip(text_buffer);
    EXPECT_STREQ(text_buffer, "hel lo");
  }

  /* With other whitespaces */
  {
    const char* sample_text = " \n h el  lo\n \0";
    const int sample_text_len = strlen(sample_text);
    size_t sample_text_size = sizeof(char)*sample_text_len;

    char text_buffer[32];
    memset(text_buffer, '\0', sizeof(text_buffer));
    memcpy(text_buffer, sample_text, sample_text_size);

    string_strip(text_buffer);
    EXPECT_STREQ(text_buffer, "h el  lo");
  }
}

TEST(UtilsTest, StringNormalize) {
  const char* sample_text = " \n hel  lo  a     world \n\n \n \n  \n    ";
  const int sample_text_len = strlen(sample_text);
  size_t sample_text_size = sizeof(char)*sample_text_len;

  char text_buffer[32];
  memset(text_buffer, '\0', sizeof(text_buffer));
  memcpy(text_buffer, sample_text, sample_text_size);

  string_normalize(text_buffer);
  EXPECT_STREQ(text_buffer, "hel lo a world");
}

TEST(UtilsTest, StringCopyTrimed) {
  /* Fitting into buffer */
  {
    char text_buffer[8];
    memset(text_buffer, '\0', sizeof(text_buffer));
    const char* some_text = "hello";
    overflow_t copy_result = cpystr_trimed(text_buffer, some_text, 8);
    EXPECT_EQ(copy_result, OVERFLOW_NONE);
    EXPECT_STREQ(text_buffer, "hello");
  }

  /* Overflow buffer */
  {
    char text_buffer[8];
    memset(text_buffer, '\0', sizeof(text_buffer));
    const char* some_text = "hello world";
    overflow_t copy_result = cpystr_trimed(text_buffer, some_text, 8);
    EXPECT_EQ(copy_result, OVERFLOW_TRUNCATED);
    EXPECT_STREQ(text_buffer, "hello w");
  }
}

TEST(UtilsTest, StringNoLeadingSpaces) {
  {
    const char* sample_text = " \n hello world";
    char* result = string_get_noleading_whitespace_start(sample_text);
    EXPECT_STREQ(result, "hello world");
  }
  {
    const char* null_text = NULL;
    char* result = string_get_noleading_whitespace_start(null_text);
    EXPECT_TRUE(result == NULL);
  }
  {
    const char* empty_text = "";
    char* result = string_get_noleading_whitespace_start(empty_text);
    EXPECT_TRUE(result == NULL);
  }
}

/* String comparing and matching */
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


/* Word iteration */
TEST(UtilsTest, WordsIteration) {
  word_iterator_t word_split_iterator;

  /* Expected behaviour */
  const char* some_str = "Hello world, how are you?";
  word_iterator_start(&word_split_iterator, some_str);
  int word_start_index = 0;
  {
    option_t split_result_1 = word_iterator_has_next(&word_split_iterator);
    EXPECT_EQ(split_result_1, OPTION_SOME);
    EXPECT_STREQ(word_split_iterator.next_word, "Hello");
    EXPECT_STREQ(word_split_iterator.next_word_src, "world, how are you?");
    EXPECT_EQ(word_split_iterator.next_word_index, 0);
    EXPECT_STREQ(word_split_iterator.src_text, some_str);

    int word_length = strlen(word_split_iterator.next_word);
    EXPECT_EQ(word_split_iterator.next_word_length, word_length);
    word_start_index += word_length + 1;
    EXPECT_EQ(word_split_iterator.current_char_index, word_start_index);
  }
  {
    option_t split_result_2 = word_iterator_has_next(&word_split_iterator);
    EXPECT_EQ(split_result_2, OPTION_SOME);
    EXPECT_STREQ(word_split_iterator.next_word, "world,");
    EXPECT_STREQ(word_split_iterator.next_word_src, "how are you?");
    EXPECT_EQ(word_split_iterator.next_word_index, 1);
    EXPECT_STREQ(word_split_iterator.src_text, some_str);
    
    int word_length = strlen(word_split_iterator.next_word);
    EXPECT_EQ(word_split_iterator.next_word_length, word_length);
    word_start_index += word_length + 1;
    EXPECT_EQ(word_split_iterator.current_char_index, word_start_index);
  }
  {
    option_t split_result_3 = word_iterator_has_next(&word_split_iterator);
    EXPECT_EQ(split_result_3, OPTION_SOME);
    EXPECT_STREQ(word_split_iterator.next_word, "how");
    EXPECT_STREQ(word_split_iterator.next_word_src, "are you?");
    EXPECT_EQ(word_split_iterator.next_word_index, 2);
    EXPECT_STREQ(word_split_iterator.src_text, some_str);
    
    int word_length = strlen(word_split_iterator.next_word);
    EXPECT_EQ(word_split_iterator.next_word_length, word_length);
    word_start_index += word_length + 1;
    EXPECT_EQ(word_split_iterator.current_char_index, word_start_index);
  }
  {
    option_t split_result_4 = word_iterator_has_next(&word_split_iterator);
    EXPECT_EQ(split_result_4, OPTION_SOME);
    EXPECT_STREQ(word_split_iterator.next_word, "are");
    EXPECT_STREQ(word_split_iterator.next_word_src, "you?");
    EXPECT_EQ(word_split_iterator.next_word_index, 3);
    EXPECT_STREQ(word_split_iterator.src_text, some_str);
    
    int word_length = strlen(word_split_iterator.next_word);
    EXPECT_EQ(word_split_iterator.next_word_length, word_length);
    word_start_index += word_length + 1;
    EXPECT_EQ(word_split_iterator.current_char_index, word_start_index);
  }
  {
    option_t split_result_5 = word_iterator_has_next(&word_split_iterator);
    EXPECT_EQ(split_result_5, OPTION_SOME);
    EXPECT_STREQ(word_split_iterator.next_word, "you?");
    EXPECT_STREQ(word_split_iterator.next_word_src, "");
    EXPECT_EQ(word_split_iterator.next_word_index, 4);
    EXPECT_STREQ(word_split_iterator.src_text, some_str);
    
    int word_length = strlen(word_split_iterator.next_word);
    EXPECT_EQ(word_split_iterator.next_word_length, word_length);
    /* Watch out - here is end of word - no +1*/
    word_start_index += word_length;
    EXPECT_EQ(word_split_iterator.current_char_index, word_start_index);
  }
  {
    option_t split_result_6 = word_iterator_has_next(&word_split_iterator);
    EXPECT_EQ(split_result_6, OPTION_NONE);
  }

  /* Remove trailing spaces. Note - multiple whitespaces should be removed by normalizing. */
  const char* other_string = "go home  \n";
  word_iterator_start(&word_split_iterator, other_string);
  {
    option_t split_result_1 = word_iterator_has_next(&word_split_iterator);
    EXPECT_EQ(split_result_1, OPTION_SOME);
    EXPECT_STREQ(word_split_iterator.next_word, "go");
    EXPECT_STREQ(word_split_iterator.next_word_src, "home  \n");
  }
  {
    option_t split_result_2 = word_iterator_has_next(&word_split_iterator);
    EXPECT_EQ(split_result_2, OPTION_SOME);
    EXPECT_STREQ(word_split_iterator.next_word, "home");
    /* Watchout - space is removed */
    EXPECT_STREQ(word_split_iterator.next_word_src, "");
  }
  {
    option_t split_result_3 = word_iterator_has_next(&word_split_iterator);
    EXPECT_EQ(split_result_3, OPTION_NONE);
  }

  /* Remove leading spaces */
  const char* leading_space_string = "  go   home \n ";
  word_iterator_start(&word_split_iterator, leading_space_string);
  {
    option_t split_result_1 = word_iterator_has_next(&word_split_iterator);
    EXPECT_EQ(split_result_1, OPTION_SOME);
    EXPECT_STREQ(word_split_iterator.next_word, "go");
    EXPECT_STREQ(word_split_iterator.next_word_src, "home \n ");
  }
  {
    option_t split_result_2 = word_iterator_has_next(&word_split_iterator);
    EXPECT_EQ(split_result_2, OPTION_SOME);
    EXPECT_STREQ(word_split_iterator.next_word, "home");
    EXPECT_STREQ(word_split_iterator.next_word_src, "");
  }
  {
    option_t split_result_3 = word_iterator_has_next(&word_split_iterator);
    EXPECT_EQ(split_result_3, OPTION_NONE);
  }

  /* Remove leading spaces */
  const char* leading_newline_string = " \n  g o \n ";
  word_iterator_start(&word_split_iterator, leading_newline_string);
  {
    option_t split_result_1 = word_iterator_has_next(&word_split_iterator);
    EXPECT_EQ(split_result_1, OPTION_SOME);
    EXPECT_STREQ(word_split_iterator.next_word, "g");
    EXPECT_STREQ(word_split_iterator.next_word_src, "o \n ");
  }  
  {
    option_t split_result_2 = word_iterator_has_next(&word_split_iterator);
    EXPECT_EQ(split_result_2, OPTION_SOME);
    EXPECT_STREQ(word_split_iterator.next_word, "o");
    EXPECT_STREQ(word_split_iterator.next_word_src, "");
  }
  {
    option_t split_result_3 = word_iterator_has_next(&word_split_iterator);
    EXPECT_EQ(split_result_3, OPTION_NONE);
  }
}

