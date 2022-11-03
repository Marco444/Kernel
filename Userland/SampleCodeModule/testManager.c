

#include "include/testManager.h"
#include "include/commands.h"
#include "include/commandsEngine.h"
#include "include/stdio.h"

#define MM_MAX_MEMORY 0x900000

void passed(char *testName) {
  putsf_(" DONE : ", GREEN);
  puts_(testName);
}

void printTest(char *str) { printHeader(str, WHITE | GREEN_BACKGROUND); }

void testManagerRun(int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {

  if (argc == 2 && strcmp_(argv[1], "memory") == 0) {
    printTest("                     Running Memory manager test ... ");
    testMM(-1, MM_MAX_MEMORY);

  } else if (argc == 2 && strcmp_(argv[1], "processes") == 0) {
    printTest("                     Running Processes test ...");
    testProcesses(argc, argv);
  } else if (argc == 2 && strcmp_(argv[1], "sync") == 0) {
    printTest("                     Running Syncronisation test ... ");
    char arguments[2][2] = {"3", "1"};
    testSync(2, arguments);
  } else if (argc == 2 && strcmp_(argv[1], "prio") == 0) {
    printTest("                     Running Priorities test ... ");
    testPrio();
  } else {
    puts_("No ingreso el tipo de argumentos validos \n");
    puts_(TEST_MANAGER_DESCRIPTION);
  }
}
