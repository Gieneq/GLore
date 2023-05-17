#pragma once
#include "utils.h"

#define RESPONSE_BUFFER_SIZE 128
#define RESPONSE_MAX_LENGTH (RESPONSE_BUFFER_SIZE - 1)


typedef struct response_t {
    char text [RESPONSE_BUFFER_SIZE];
    bool_t has_wildcard;
} response_t;

void response_init(response_t* response);
result_t response_from_string(response_t* response, const char *str, bool_t has_wildcard);