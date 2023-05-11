#pragma once
#include <stdio.h>

#define DEBUG 0

#if defined(DEBUG) && DEBUG == 1
#define debug_printf(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#define debug_printf(fmt, ...)
#endif