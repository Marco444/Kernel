

#include "include/testManager.h"
#include "include/stdio.h"

#define MM_MAX_MEMORY 0xA000000

void passed(char *testName, Window window) {
  putsf_(" DONE : ", GREEN);
  puts_(testName);
}

void testManagerRun(Window window, int argc,
                    char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {

  // recibe mal los argumentos al ejecutarse en background las cosas!
  // for (int i = 0; i < argc; i++) {
  //   putInteger(i);
  //   puts_(" : ");
  //   puts_(argv[i]);
  //   newLine(window);
  // }

  // testProcesses(window, argc, argv);
  testMM(window, -1, MM_MAX_MEMORY);
  return;

  if (argc == 2 && strcmp_(argv[1], "memory") == 0) {

    puts_("Running memory manager test ... \n");
    testMM(window, -1, MM_MAX_MEMORY);

  } else if (argc == 2 && strcmp_(argv[1], "processes") == 0) {
    puts_("Running processes test ...");
    testProcesses(window, argc, argv);
  } else {
    // testProcesses(window, argc, argv);
    puts_("Running Syncronisation test ... \n");
    char arguments[2][2] = {"3", "1"};
    testSync(2, arguments);
  }
}
