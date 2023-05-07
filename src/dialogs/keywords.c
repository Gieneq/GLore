#include "keywords.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Keyword */
void keyword_init(keyword_t* keyword) {
    memset(keyword->text, '\0', KEYWORD_BUFFER_SIZE);
    keyword->length = 0;
}

result_t keyword_from_string(keyword_t* keyword, const char *str) {
    keyword_init(keyword);
    if (strlen(str) > KEYWORD_MAX_LENGTH) {
        printf("Error: Keyword too long");
        return RESULT_ERROR;
    }
    keyword->length = strlen(str);
    strncpy(keyword->text, str, KEYWORD_MAX_LENGTH);
    return RESULT_OK;
}


/* Keywords */
void keywords_list_init(keywords_list_t *list) {
    list->count = 0;
    for (int i = 0; i < KEYWORDS_MAX_COUNT; i++) {
        keyword_init(&list->keywords[i]);
    }
}

result_t keywords_list_from_array(keywords_list_t *list, const char *keywords[], int count) {
    keywords_list_init(list);
    if (count > KEYWORDS_MAX_COUNT) {
        printf("Error: Too many keywords");
        return RESULT_ERROR;
    }
    for (int i = 0; i < count; i++) {
        keyword_from_string(&list->keywords[i], keywords[i]);
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
        keyword_from_string(&list->keywords[i], token);
        token = strtok(NULL, delim);
        i++;
    }
    list->count = i;
    free(str_copy);
    return RESULT_OK;
}

void keywords_list_printf(const keywords_list_t *list) {
    for (int i = 0; i < list->count; i++) {
        printf("%s ", list->keywords[i].text);
    }
    printf("\n");
}


/* Matching */
option_t keyword_match(const keyword_t* keyword, const char *str) {
    // printf("..searching /%s/%s/\n", keyword->text, str);
    int str_length = strlen(str);
    if(keyword->length > str_length) {
        return OPTION_NONE;
    }

    for(int i=0; i<str_length - keyword->length + 1; ++i) {
        // int match = strcmp(keyword->text, str + i);
        // printf("...match %d: /%s/%s/\n", match, keyword->text, str + i);
        if (strstr(str + i, keyword->text)) {
            // printf("....found\n");
            return OPTION_SOME;
        }
    }

    return OPTION_NONE;
}


option_t keyword_match_front(const keyword_t* keyword, const char *str) {
    if(keyword->length > strlen(str)) {
        return OPTION_NONE;
    }

    if (strstr(str, keyword->text)) {
        return OPTION_SOME;
    }
    // printf(" %s vs kw: ..%s..\n", str, keyword->text);
    // if ((str[0] == keyword->text[0]) && (str[1] == keyword->text[1])) {
    //     return OPTION_SOME;
    // }

    return OPTION_NONE;
}

option_t keywords_list_match(const keywords_list_t *list, const char *str) {
    for (int i = 0; i < list->count; i++) {
        if (keyword_match(&list->keywords[i], str) == OPTION_SOME) {
            return OPTION_SOME;
        }
    }
    return OPTION_NONE;
}

option_t keywords_list_match_front(const keywords_list_t* list, const char *str) {
    for (int i = 0; i < list->count; i++) {
        // printf("Cecking %s/%s is %d\n", list->keywords[i].text, str, (int)(keyword_match_front(&list->keywords[i], str)));
        if (keyword_match_front(&list->keywords[i], str) == OPTION_SOME) {
            return OPTION_SOME;
        }
    }
    return OPTION_NONE;
}