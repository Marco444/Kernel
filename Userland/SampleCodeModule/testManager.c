

#include "include/testManager.h"
#include "include/stdio.h"

void passed(char *testName, Window window) {
  putsf_(" DONE : ", GREEN, window);
  puts_(testName, window);
}

void testManagerRun(Window window, int argc,
                    char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {

  // recibe mal los argumentos al ejecutarse en background las cosas!
  // for (int i = 0; i < argc; i++) {
  //   putInteger(i, window);
  //   puts_(" : ", window);
  //   puts_(argv[i], window);
  //   newLine(window);
  // }

  // testProcesses(window, argc, argv);
  testMM(window, -1, 0xA000000);
  // return;

  if (argc == 2 && strcmp_(argv[1], "memory") == 0) {

    puts_("Running memory manager test ... \n", window);
    testMM(window, -1, 10);

  } else if (argc == 2 && strcmp_(argv[1], "processes") == 0) {
    puts_("Running processes test ...", window);
    testProcesses(window, argc, argv);
  } else {
    // testProcesses(window, argc, argv);
    puts_("Running Syncronisation test ... \n", window);
    char arguments[2][2] = {"3", "1"};
    testSync(2, arguments);
  }
}
