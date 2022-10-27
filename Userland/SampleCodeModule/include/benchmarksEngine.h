

#include "stdio.h"
#include "stdlib.h"

#define MAX_ARGUMENT 20
#define MAX_ARGUMENT_COUNT 20

void stressTest(Window window);

void benchmarkMemoryManager(Window window, int argc,
                            char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]);
