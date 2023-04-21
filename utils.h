#pragma once

#define MAX(a, b) ((a) > (b)  ? (a) : (b))
#define MIN(a, b) ((a) < (b)  ? (a) : (b))
#define ABS(a)    ((a) < 0    ? -(a) : (a))
#define SIGN(a)   ((a) < 0    ? -1 : 1)
#define CLAMP(a, b, c) (MAX(b, MIN(a, c)))


typedef enum result {
    RESULT_OK,
    RESULT_ERROR,
} result_t;


