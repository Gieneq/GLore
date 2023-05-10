#pragma once

typedef enum result {
    RESULT_OK,
    RESULT_ERROR,
} result_t;

typedef enum bool {
    FALSE,
    TRUE,
} bool_t;

typedef enum option {
    OPTION_SOME,
    OPTION_NONE,
} option_t;

typedef enum overflow {
    OVERFLOW,
    NO_OVERFLOW,
} overflow_t;


#define iterator_foreach(element, iterator) for(*(element) = ((iterator)->first != NULL) ? (iterator)->first : NULL; ((iterator)->current < (iterator)->count) && *(element); *(element) = (iterator)->first + ++((iterator)->current))

#define WORD_ITERATOR_BUFFER_SIZE 128
#define WORD_ITERATOR_MAX_LENGTH (INPUT_WORD_BUFFER_SIZE - 1)

typedef struct word_iterator_t {
    const char* src_text;
    char next_word[WORD_ITERATOR_BUFFER_SIZE];
    const char* next_word_src;
    int next_word_length;
    int current_char_index;
    int next_word_index;
}   word_iterator_t;

void word_iterator_start(word_iterator_t* word_iterator, const char* text);
option_t word_iterator_has_next(word_iterator_t* word_iterator);

#define MAX(a, b) ((a) > (b)  ? (a) : (b))
#define MIN(a, b) ((a) < (b)  ? (a) : (b))
#define ABS(a)    ((a) < 0    ? -(a) : (a))
#define SIGN(a)   ((a) < 0    ? -1 : 1)
#define CLAMP(a, b, c) (MAX(b, MIN(a, c)))


/* copy string with l=characters length constrain macro */
overflow_t cpystr_trimed(char *dst, const char *src, const size_t buffer_size);
option_t match_string_ignorecase(const char* src, const char* substring);
char tolowercase(char c);
option_t string_compare_ignorecase(const char* src, const char* dst);