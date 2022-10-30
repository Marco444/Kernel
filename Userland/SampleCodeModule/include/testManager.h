

#include "_string.h"
#include "constants.h"
#include "stdio.h"
#include "stdlib.h"
#include "testUtil.h"

#include <string.h>

void passed(char *testName, Window window);

void testMM(Window window, int iterations, int maxMemory);

void testProcesses(Window window, int argc,
                   char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]);

void testManagerRun(Window window, int argc,
                    char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]);

//uint64_t testSync(uint64_t argc, char *argv[]);
