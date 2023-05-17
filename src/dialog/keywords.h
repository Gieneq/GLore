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

/* Keywords list */
typedef struct keywords_list_t {
    keyword_t keywords[KEYWORDS_MAX_COUNT];
    int count;
} keywords_list_t;
void keywords_list_init(keywords_list_t *list);
result_t keywords_list_from_array(keywords_list_t *list, const char *keywords[], int count);
result_t keywords_list_from_delimited_string(keywords_list_t *list, const char *str, const char *delim);
void keywords_list_printf(const keywords_list_t *list);

/* Keyword wildcard */
typedef enum wildcard_type_t {
    WILDCARD_TYPE_NONE = 0,
    WILDCARD_TYPE_PLAYER_NAME,
    WILDCARD_TYPE_NPC_NAME,
    WILDCARD_TYPE_QUEST_NAME,
    WILDCARD_TYPE_ITEM_NAME,
} wildcard_type_t;

typedef struct keyword_wildcard_t {
    wildcard_type_t type;
    // union {
    int id;
    // };
} keyword_wildcard_t;

void keyword_wildcard_init(keyword_wildcard_t *kw, const wildcard_type_t type);

/* Matchings */

option_t keyword_match_any_ignorecase(const keyword_t* keyword, const char *str);

option_t keyword_match_front_ignorecase(const keyword_t* keyword, const char *str);

option_t keywords_list_match_any_ignorecase(const keywords_list_t *list, const char *str);

option_t keywords_list_match_front_ignorecase(const keywords_list_t* list, const char *str);
