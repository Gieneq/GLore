#pragma once
#include <utils.h>
#include "cJSON.h"
#define KEYWORD_BUFFER_SIZE 64
#define KEYWORD_MAX_LENGTH (KEYWORD_BUFFER_SIZE - 1)
#define KEYWORDS_MAX_COUNT 4

/* Keyword */
// typedef char keyword_t[KEYWORD_BUFFER_SIZE];
typedef struct keyword_t {
    char text[KEYWORD_BUFFER_SIZE];
    int length;
} keyword_t;

void keyword_init(keyword_t* keyword);
result_t keyword_from_string(keyword_t* keyword, const char *str);

/* Keywords */
typedef struct keywords_list_t {
    keyword_t keywords[KEYWORDS_MAX_COUNT];
    int count;
} keywords_list_t;
void keywords_list_init(keywords_list_t *list);
result_t keywords_list_from_array(keywords_list_t *list, const char *keywords[], int count);
result_t keywords_list_from_delimited_string(keywords_list_t *list, const char *str, const char *delim);
void keywords_list_printf(const keywords_list_t *list);

/* Matchings */
option_t keyword_match(const keyword_t* keyword, const char *str);
option_t keyword_match_front(const keyword_t* keyword, const char *str);
option_t keywords_list_match(const keywords_list_t *list, const char *str);
option_t keywords_list_match_front(const keywords_list_t* list, const char *str);
