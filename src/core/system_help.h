#pragma once
#include "utils.h"
#include "keywords.h"

#define HELP_COMMANDS_MAX_COUNT 32

#define HELP_COMMAND_DESCRIPTION_BUFFER_SIZE 128
#define HELP_COMMAND_DESCRIPTION_MAX_LENGTH (HELP_COMMAND_DESCRIPTION_BUFFER_SIZE - 1)
#define HELP_COMMAND_INITIAL_TEXT_BUFFER_SIZE 128
#define HELP_COMMAND_INITIAL_TEXT_MAX_LENGTH (HELP_COMMAND_INITIAL_TEXT_BUFFER_SIZE - 1)
#define HELP_COMMAND_ENDING_TEXT_BUFFER_SIZE 128
#define HELP_COMMAND_ENDING_TEXT_MAX_LENGTH (HELP_COMMAND_ENDING_TEXT_BUFFER_SIZE - 1)

typedef enum help_command_type_t {
    HELP_COMMAND_TYPE_KEYWORD,
    HELP_COMMAND_TYPE_KEYWORDS_LIST,
} help_command_type_t;

typedef struct help_cmd_t {
    union {
        keyword_t keyword;
        keywords_list_t keywords_list;
    };
    help_command_type_t type;
    char description[HELP_COMMAND_DESCRIPTION_BUFFER_SIZE];
} help_cmd_t;

result_t system_help_create_cmd_with_keyword(help_cmd_t* cmd, const keyword_t* keyword, const char* description);
result_t system_help_create_cmd_with_keywords_list(help_cmd_t* cmd, const keywords_list_t* keywords_list, const char* description);

typedef struct help_t {
    help_cmd_t commands_register[HELP_COMMANDS_MAX_COUNT];
    int commands_count;
    char initial_text[HELP_COMMAND_INITIAL_TEXT_BUFFER_SIZE];
    char ending_text[HELP_COMMAND_ENDING_TEXT_BUFFER_SIZE];
} help_t;

result_t system_help_init(help_t* help);
result_t system_help_append_command_with_keyword(help_t* help, const keyword_t* keyword, const char* description);
result_t system_help_append_command_with_keywords_list(help_t* help, const keywords_list_t* keywords_list, const char* description);
result_t system_help_set_initial_text(help_t* help, const char* initial_text);
result_t system_help_set_ending_text(help_t* help, const char* ending_text);
void system_help_print(const help_t* help);