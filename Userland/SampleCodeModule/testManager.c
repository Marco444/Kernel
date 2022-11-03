

#include "include/testManager.h"
#include "include/stdio.h"

#define MM_MAX_MEMORY 0x900000

void passed(char *testName) {
  putsf_(" DONE : ", GREEN);
  puts_(testName);
}

void printTest(char *str) { printHeader(str, WHITE | LIGHT_GREEN_BACKGROUND); }

void testManagerRun(int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {

  if (argc == 2 && strcmp_(argv[1], "memory") == 0) {
    printTest("                 Running memory manager test ... ");
    testMM(-1, MM_MAX_MEMORY);

  } else if (argc == 2 && strcmp_(argv[1], "processes") == 0) {
    printTest("Running processes test ...");
    testProcesses(argc, argv);
  } else if (argc == 2 && strcmp_(argv[1], "sync") == 0) {
    printTest("      Running Syncronisation test ... ");
    char arguments[2][2] = {"3", "1"};
    testSync(2, arguments);
  } else if (argc == 2 && strcmp_(argv[1], "prio") == 0) {
    printTest("      Running Priorities test ... ");
    testPrio();
  }
}
