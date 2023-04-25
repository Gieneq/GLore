#include "keywords.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Keyword */
void keyword_init(keyword_t keyword) {
    memset(keyword, '\0', KEYWORD_BUFFER_SIZE);
}

result_t keyword_from_string(keyword_t keyword, const char *str) {
    keyword_init(keyword);
    if (strlen(str) > KEYWORD_MAX_LENGTH) {
        printf("Error: Keyword too long");
        return RESULT_ERROR;
    }
    strncpy(keyword, str, KEYWORD_MAX_LENGTH);
    return RESULT_OK;
}


/* Keywords */
void keywords_list_init(keywords_list_t *list) {
    list->count = 0;
    for (int i = 0; i < KEYWORDS_MAX_COUNT; i++) {
        keyword_init(list->keywords[i]);
    }
}

result_t keywords_list_from_array(keywords_list_t *list, const char *keywords[], int count) {
    keywords_list_init(list);
    if (count > KEYWORDS_MAX_COUNT) {
        printf("Error: Too many keywords");
        return RESULT_ERROR;
    }
    for (int i = 0; i < count; i++) {
        keyword_from_string(list->keywords[i], keywords[i]);
    }
    list->count = count;
    return RESULT_OK;
}

result_t keywords_list_from_delimited_string(keywords_list_t *list, const char *str, const char *delim) {
    keywords_list_init(list);
    char *str_copy = strdup(str);
    char *token = strtok(str_copy, delim);
    int i = 0;
    while (token != NULL && i < KEYWORDS_MAX_COUNT) {
        keyword_from_string(list->keywords[i], token);
        token = strtok(NULL, delim);
        i++;
    }
    list->count = i;
    free(str_copy);
    return RESULT_OK;
}

void keywords_list_printf(const keywords_list_t *list) {
    for (int i = 0; i < list->count; i++) {
        printf("%s ", list->keywords[i]);
    }
    printf("\n");
}


/* Matching */
option_t keyword_match(const keyword_t keyword, const char *str) {
    if (strcmp(keyword, str) == 0) {
        return OPTION_SOME;
    }
    return OPTION_NONE;
}

option_t keywords_list_match(const keywords_list_t *list, const char *str) {
    for (int i = 0; i < list->count; i++) {
        if (keyword_match(list->keywords[i], str) == OPTION_SOME) {
            return OPTION_SOME;
        }
    }
    return OPTION_NONE;
}