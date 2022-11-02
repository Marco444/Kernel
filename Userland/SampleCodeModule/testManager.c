

#include "include/testManager.h"
#include "include/stdio.h"

#define MM_MAX_MEMORY 0xA000000

void passed(char *testName) {
  putsf_(" DONE : ", GREEN);
  puts_(testName);
}

void testManagerRun(int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {

  if (argc == 2 && strcmp_(argv[1], "memory") == 0) {
    puts_("Running memory manager test ... \n");
    testMM(-1, MM_MAX_MEMORY);

  } else if (argc == 2 && strcmp_(argv[1], "processes") == 0) {
    puts_("Running processes test ...");
    testProcesses(argc, argv);
  } else if (argc == 2 && strcmp_(argv[1], "sync") == 0) {
    puts_("Running Syncronisation test ... \n");
    char arguments[2][2] = {"3", "1"};
    testSync(2, arguments);
  } else if (argc == 2 && strcmp_(argv[1], "prio") == 0) {
    puts_("Running Priorities test ... \n");
    testPrio();
  }
}
