#include "responses.h"
#include <string.h>
#include <stdio.h>

void response_init(response_t* response) {
    memset(response->text, '\0', RESPONSE_BUFFER_SIZE);
    response->has_wildcard = BOOL_FALSE;
}

result_t response_from_string(response_t* response, const char *str, bool_t has_wildcard) {
    response_init(response);
    if (str == NULL) {
        printf("Error: Response string is NULL");
        return RESULT_ERROR;
    }
    if (strlen(str) >= RESPONSE_MAX_LENGTH) {
        printf("Error: Response string is too long");
        return RESULT_ERROR;
    }
    strcpy(response->text, str);
    response->has_wildcard = has_wildcard;
    return RESULT_OK;
}