#include "responses.h"
#include <string.h>
#include <stdio.h>

void response_init(response_t response) {
    memset(response, '\0', RESPONSE_BUFFER_SIZE);
}

result_t response_from_string(response_t response, const char *str) {
    response_init(response);
    if (str == NULL) {
        printf("Error: Response string is NULL");
        return RESULT_ERROR;
    }
    if (strlen(str) >= RESPONSE_MAX_LENGTH) {
        printf("Error: Response string is too long");
        return RESULT_ERROR;
    }
    strcpy(response, str);
    return RESULT_OK;
}