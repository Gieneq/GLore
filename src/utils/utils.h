#pragma once
#include <stdio.h>

/* PLATFORMS */
#define PLATFORM_WIN 0 

/* Global settings */
#define PLATFORM PLATFORM_WIN
#define DEBUG 0
#define ERROR_OUTPUT 1

/* Derived settings */
#if PLATFORM == PLATFORM_WIN
#define FANCY_CMD 1
#else
#define FANCY_CMD 0
#endif

/* Fancy CMD */
void fancy_cmd_init();

#if FANCY_CMD == 1
#include <windows.h>
typedef struct fancy_cmd_t {
    HANDLE hConsole;
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;
} fancy_cmd_t;

extern fancy_cmd_t _utils_fancy_cmd;

#define fancy_cmd_set_black() SetConsoleTextAttribute(_utils_fancy_cmd.hConsole, 0)
#define fancy_cmd_set_blue() SetConsoleTextAttribute(_utils_fancy_cmd.hConsole, 1)
#define fancy_cmd_set_green() SetConsoleTextAttribute(_utils_fancy_cmd.hConsole, 2)
#define fancy_cmd_set_turquoise() SetConsoleTextAttribute(_utils_fancy_cmd.hConsole, 3)
#define fancy_cmd_set_red() SetConsoleTextAttribute(_utils_fancy_cmd.hConsole, 4)
#define fancy_cmd_set_purple() SetConsoleTextAttribute(_utils_fancy_cmd.hConsole, 5)
#define fancy_cmd_set_golden() SetConsoleTextAttribute(_utils_fancy_cmd.hConsole, 6)
#define fancy_cmd_set_lightgray() SetConsoleTextAttribute(_utils_fancy_cmd.hConsole, 7)
#define fancy_cmd_set_gray() SetConsoleTextAttribute(_utils_fancy_cmd.hConsole, 8)
#define fancy_cmd_set_lightblue() SetConsoleTextAttribute(_utils_fancy_cmd.hConsole, 9)
#define fancy_cmd_set_lightgreen() SetConsoleTextAttribute(_utils_fancy_cmd.hConsole, 10)
#define fancy_cmd_set_aqua() SetConsoleTextAttribute(_utils_fancy_cmd.hConsole, 11)
#define fancy_cmd_set_orange() SetConsoleTextAttribute(_utils_fancy_cmd.hConsole, 12)
#define fancy_cmd_set_pink() SetConsoleTextAttribute(_utils_fancy_cmd.hConsole, 13)
#define fancy_cmd_set_ivory() SetConsoleTextAttribute(_utils_fancy_cmd.hConsole, 14)
#define fancy_cmd_set_white() SetConsoleTextAttribute(_utils_fancy_cmd.hConsole, 15)
#define fancy_cmd_set_any(color) SetConsoleTextAttribute(_utils_fancy_cmd.hConsole, color)
#define fancy_cmd_set_default() SetConsoleTextAttribute(_utils_fancy_cmd.hConsole, 15)
#endif

/* Response print */
#if FANCY_CMD == 1
#define response_printf(fmt, ...) fancy_cmd_set_golden(); printf(fmt, ##__VA_ARGS__); fancy_cmd_set_default()
#else
#define response_printf(fmt, ...) printf(fmt, ##__VA_ARGS__)
#endif

/* Narrator print */
#if FANCY_CMD == 1
#define narrator_printf(fmt, ...) fancy_cmd_set_ivory(); printf(fmt, ##__VA_ARGS__); fancy_cmd_set_default()
#else
#define narrator_printf(fmt, ...) printf(fmt, ##__VA_ARGS__)
#endif

/* Info print */
#if FANCY_CMD == 1
#define info_printf(fmt, ...) fancy_cmd_set_lightgray(); printf(fmt, ##__VA_ARGS__); fancy_cmd_set_default()
#else
#define info_printf(fmt, ...) printf(fmt, ##__VA_ARGS__)
#endif

/* Debug print */
#if DEBUG == 1
#if FANCY_CMD == 1
#define debug_printf(fmt, ...) fancy_cmd_set_blue(); printf("#DEBUG "fmt, ##__VA_ARGS__); fancy_cmd_set_default()
#else
#define debug_printf(fmt, ...) printf("#DEBUG "fmt, ##__VA_ARGS__)
#endif
#else
#define debug_printf(fmt, ...)
#endif

/* Error print */
#if ERROR_OUTPUT == 1
#if FANCY_CMD == 1
#define error_printf(fmt, ...) fancy_cmd_set_red(); printf("#ERROR "fmt, ##__VA_ARGS__); fancy_cmd_set_default()
#else
#define error_printf(fmt, ...) printf("#ERROR "fmt, ##__VA_ARGS__)
#endif
#else
#define error_printf(fmt, ...)
#endif

/* Hardfault */
#define HARDFAULT() {error_printf("Hardfault in %s at %s:%d. Exit!\n", __FILE__, __func__, __LINE__); exit(1);}

typedef enum result {
    RESULT_OK,
    RESULT_ERROR,
} result_t;

typedef enum bool {
    BOOL_FALSE,
    BOOL_TRUE,
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