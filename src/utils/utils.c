#include "utils.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

static fancy_cmd_t _utils_fancy_cmd;

void fancy_cmd_init() { 
#if defined(FANCY_CMD) && FANCY_CMD == 1
    /* Save current attributes */
    _utils_fancy_cmd.hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(_utils_fancy_cmd.hConsole, &_utils_fancy_cmd.consoleInfo);
    _utils_fancy_cmd.saved_attributes = _utils_fancy_cmd.consoleInfo.wAttributes;
#endif
}

overflow_t cpystr_trimed(char *dst, const char *src, const size_t buffer_size) {
    const size_t src_len = strlen(src);
    const size_t chars_count = MIN(src_len, buffer_size);
    memset(dst, '\0', buffer_size);
    memcpy(dst, src, chars_count);
    overflow_t result = src_len >= buffer_size - 1 ? OVERFLOW : NO_OVERFLOW;
    if(result == OVERFLOW) {
        printf("Warning: string overflowed, %llu string was trimmed to %llu characters\n", src_len, buffer_size);
    }
    return result;
}

void word_iterator_start(word_iterator_t* word_iterator, const char* text) {
    word_iterator->current_char_index = 0;
    word_iterator->next_word_length = 0;
    word_iterator->src_text = text;
    word_iterator->next_word_src = NULL;
    memset(word_iterator->next_word, '\0', WORD_ITERATOR_BUFFER_SIZE);
    word_iterator->next_word_index = -1;

    int text_len = strlen(text);    
    if(text_len <= 0) {
        word_iterator->src_text = NULL;
        return;
    }

    /* Remove leading spaces if occured */
    while(text[word_iterator->current_char_index] == ' ') {
        ++word_iterator->current_char_index;
    }
}

option_t word_iterator_has_next(word_iterator_t* word_iterator) {
    if(word_iterator->src_text == NULL) {
        return OPTION_NONE;
    }

    if(word_iterator->src_text[word_iterator->current_char_index] == '\0') {
        return OPTION_NONE;
    }

    if(word_iterator->current_char_index >= strlen(word_iterator->src_text)) {
        return OPTION_NONE;
    }

    /* Detect end of next word */
    int start_index = word_iterator->current_char_index;
    int end_index = start_index + 1;
    while(word_iterator->src_text[end_index] != ' ' && word_iterator->src_text[end_index] != '\0') {
        ++end_index;
    }
    word_iterator->current_char_index = end_index;
    
    /* Calculate word length */
    word_iterator->next_word_length = end_index - start_index;

    /* Copy word */
    memset(word_iterator->next_word, '\0', WORD_ITERATOR_BUFFER_SIZE);
    memcpy(word_iterator->next_word, word_iterator->src_text + start_index, sizeof(char) * word_iterator->next_word_length);

    /* Remove spaces if occured */
    while(word_iterator->src_text[word_iterator->current_char_index] == ' ') {
        ++word_iterator->current_char_index;
    }

    /* Move pointer to the next word */
    word_iterator->next_word_src = word_iterator->src_text + word_iterator->current_char_index;

    if(word_iterator->next_word[0] != '\0') {
        ++word_iterator->next_word_index;
        return OPTION_SOME;
    }

    return OPTION_NONE;
}


/* Same length until zero, same chars but ignorecase, e.g. 'Go' -> 'go' */
option_t string_equals_ignorecase(const char* str1, const char* str2) {
    if(str1 == NULL || str2 == NULL) {
        return OPTION_NONE;
    }

    int str1len = strlen(str1);
    int str2len = strlen(str2);

    return string_sub_equals_ignorecase(str1, str1len, str2, str2len);
}

/* Passes 2 long strings, math only substrings - same chars, same length but ignorecase. General tool. Usage e.g. "Go left", 2, 'go', 2 */
option_t string_sub_equals_ignorecase(const char* str1, const int substr1_len, const char* str2, const int substr2_len) {
    if(str1 == NULL || str2 == NULL) {
        return OPTION_NONE;
    }

    if(substr1_len != substr2_len) {
        return OPTION_NONE;
    }

    for(int i = 0; i < substr1_len; ++i) {
        if(tolower(str1[i]) != tolower(str2[i])) {
            return OPTION_NONE;
        }
    }

    return OPTION_SOME;
}

/* Find substring inside longer string ignorecase, e.g. 'I want to GO home' -> 'go */
option_t string_match_ignorecase(const char* src, const char* substring) {
    if(src == NULL || substring == NULL) {
        return OPTION_NONE;
    }

    int src_len = strlen(src);
    int substring_len = strlen(substring);

    if(src_len < substring_len) {
        return OPTION_NONE;
    }

    for(int i = 0; i < src_len - substring_len + 1; ++i) {
        if(string_sub_equals_ignorecase(src + i, substring_len, substring, substring_len) == OPTION_SOME) {
            return OPTION_SOME;
        }
    }

    return OPTION_NONE;
}

/* Check substring at the start of longer string ignorecase, e.g. 'Go left' -> 'go' */
option_t string_match_front_ignorecase(const char* src, const char* substring) {
    if(src == NULL || substring == NULL) {
        return OPTION_NONE;
    }

    int src_len = strlen(src);
    int substring_len = strlen(substring);

    if(src_len < substring_len) {
        return OPTION_NONE;
    }

    return string_sub_equals_ignorecase(src, substring_len, substring, substring_len);
}