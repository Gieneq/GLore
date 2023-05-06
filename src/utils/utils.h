#pragma once

#define iterator_foreach(element, iterator) for(*(element) = ((iterator)->first != NULL) ? (iterator)->first : NULL; ((iterator)->current < (iterator)->count) && *(element); *(element) = (iterator)->first + ++((iterator)->current))

#define MAX(a, b) ((a) > (b)  ? (a) : (b))
#define MIN(a, b) ((a) < (b)  ? (a) : (b))
#define ABS(a)    ((a) < 0    ? -(a) : (a))
#define SIGN(a)   ((a) < 0    ? -1 : 1)
#define CLAMP(a, b, c) (MAX(b, MIN(a, c)))


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

/* copy string with l=characters length constrain macro */
overflow_t cpystr_trimed(char *dst, const char *src, const size_t buffer_size);