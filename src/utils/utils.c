#include "utils.h"
#include <string.h>
#include <stdio.h>

overflow_t cpystr_trimed(char *dst, const char *src, const size_t buffer_size) {
    const size_t src_len = strlen(src);
    const size_t chars_count = MIN(src_len, buffer_size);
    memset(dst, '\0', buffer_size);
    memcpy(dst, src, chars_count);
    overflow_t result = src_len >= buffer_size - 1 ? OVERFLOW : NO_OVERFLOW;
    if(result == OVERFLOW) {
        printf("Warning: string overflowed, %llu string was trimmed to %llu characters\n", src_len, buffer_size);
    }
    return result;
}