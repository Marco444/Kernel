

#include "_stdlib.h"
#include "_string.h"
#include "constants.h"
#include "stdio.h"
#include "testUtil.h"

#include <string.h>

void passed(char *testName, Window window);

void testMM(int iterations, int maxMemory);

void testProcesses(int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]);

void testManagerRun(int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]);

void testSync(uint64_t argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]);
