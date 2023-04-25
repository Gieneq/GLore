#pragma once
#include "utils.h"

#define RESPONSE_BUFFER_SIZE 1024
#define RESPONSE_MAX_LENGTH (RESPONSE_BUFFER_SIZE - 1)

typedef char response_t[RESPONSE_BUFFER_SIZE];

void response_init(response_t response);
result_t response_from_string(response_t response, const char *str);