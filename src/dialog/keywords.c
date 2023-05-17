#include "keywords.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static word_iterator_t kw_word_split_iterator;

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

void keyword_wildcard_init(keyword_wildcard_t *kw, const wildcard_type_t type) {
    kw->type = type;
    kw->id = 0; //or memset 0 before
}


/* Matching */

/* Match any word in the sentence, e.g. 'go left' matches 'I want to go left'. */
option_t keyword_match_any_ignorecase(const keyword_t* keyword, const char *str) {
    if(!keyword || !str) {
        return OPTION_NONE;
    }

    if(keyword_match_front_ignorecase(keyword, str) == OPTION_SOME) {
        return OPTION_SOME;
    }

    word_iterator_start(&kw_word_split_iterator, str);
    while(word_iterator_has_next(&kw_word_split_iterator) == OPTION_SOME) {
        const char* word = kw_word_split_iterator.next_word;
        const char* remaining = kw_word_split_iterator.next_word_src;
        const int word_index = kw_word_split_iterator.next_word_index;

        if(keyword_match_front_ignorecase(keyword, remaining) == OPTION_SOME) {
            return OPTION_SOME;
        }
    }

    return OPTION_NONE;
}

/* Match first word in the sentence, e.g. 'go left' matches 'go left now'. */
option_t keyword_match_front_ignorecase(const keyword_t* keyword, const char *str) {
    int str_len = strlen(str);
    // if(keyword->length < str_len) {
    //     return OPTION_NONE;
    // }

    // if(keyword->length == str_len) {
    //     if(string_equals_ignorecase(keyword->text, str) == OPTION_SOME) {
    //         return OPTION_SOME;
    //     }
    //     return OPTION_NONE;
    // }

    if(string_match_front_ignorecase(str, keyword->text) == OPTION_SOME) {
        if((keyword->length > str_len) && str[str_len] == ' ') {
            return OPTION_SOME;
        }
        return OPTION_NONE;
    }

    // if(str[str_len] == ' ') {
    //     return OPTION_SOME;
    // }
    return OPTION_NONE;
}


/* Match any word in the sentence, e.g. ['move', 'go'] matches 'I want to go left'. */
option_t keywords_list_match_any_ignorecase(const keywords_list_t *list, const char *str) {
    // #error
    for (int i = 0; i < list->count; i++) {
        if (keyword_match_any_ignorecase(&list->keywords[i], str) == OPTION_SOME) {
            return OPTION_SOME;
        }
    }
    return OPTION_NONE;
}

/* Match first word in the sentence, e.g. ['move', 'go'] matches 'go left'. */
option_t keywords_list_match_front_ignorecase(const keywords_list_t* list, const char *str) {
    // #error
    for (int i = 0; i < list->count; i++) {
        if (keyword_match_front_ignorecase(&list->keywords[i], str) == OPTION_SOME) {
            return OPTION_SOME;
        }
    }
    return OPTION_NONE;
}




// option_t keyword_match_any_ignorecase(const keyword_t* keyword, const char *str) {
//     if(!keyword || !str) {
//         return OPTION_NONE;
//     }

//     return string_contains_ignorecase(str, keyword->text);
// }





/* */
// option_t keyword_match_exact(const keyword_t* keyword, const char *str) {
//     int str_length = strlen(str);
//     if(keyword->length > str_length) {
//         return OPTION_NONE;
//     }

//     for(int i=0; i<str_length - keyword->length + 1; ++i) {
//         if (strstr(str + i, keyword->text)) {
//             return OPTION_SOME;
//         }
//     }

//     return OPTION_NONE;
// }

// option_t keyword_match_exact_ignorecase(const keyword_t* keyword, const char *str) {
//     if(!keyword || !str) {
//         return OPTION_NONE;
//     }

//     return string_equals_ignorecase(keyword->text, str);
// }

// option_t keyword_match_front_ignorecase(const keyword_t* keyword, const char *str) {
//     if(keyword->length > strlen(str)) {
//         return OPTION_NONE;
//     }

//     if (strstr(str, keyword->text)) {
//         return OPTION_SOME;
//     }

//     return OPTION_NONE;
// }


// /* Lists */
// option_t keywords_list_match_any_exact_ignorecase(const keywords_list_t *list, const char *str) {
//     for (int i = 0; i < list->count; i++) {
//         if (keyword_match_exact(&list->keywords[i], str) == OPTION_SOME) {
//             return OPTION_SOME;
//         }
//     }
//     return OPTION_NONE;
// }

// option_t keywords_list_match_front_ignorecase(const keywords_list_t* list, const char *str) {
//     for (int i = 0; i < list->count; i++) {
//         if (keyword_match_front_ignorecase(&list->keywords[i], str) == OPTION_SOME) {
//             return OPTION_SOME;
//         }
//     }
//     return OPTION_NONE;
// }

// option_t keywords_list_exact_match_any_ignorecase(const keywords_list_t* list, const char *str) {
//     word_iterator_start(&kw_word_split_iterator, str);

//     if()

//     while(word_iterator_has_next(&kw_word_split_iterator) == OPTION_SOME) {
//         const char* word = kw_word_split_iterator.next_word;
//         const char* remaining = kw_word_split_iterator.next_word_src;
//         const int word_index = kw_word_split_iterator.next_word_index;

//         /* Check if first word matches keword. Checking index is redundant. */
//         if(word_index == 0 && keyword_exact_match_ignorecase(&kw_go, word) == OPTION_SOME) {
//             if(current_room->adjecent_rooms_count == 0) {
//                 info_printf("You can't go anywhere from here.\n");
//                 return OPTION_SOME;
//             }
//         }
//     }

//     return OPTION_NONE;
// }
