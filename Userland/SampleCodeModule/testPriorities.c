// #include <stdint.h>
// #include <stdio.h>

// #define MINORWAIT \
//   1000000 // TODO: Change this value to prevent a process from flooding the
//           // screen
// #define WAIT \
//   10000000 // TODO: Change this value to make the wait long enough to see
//            // processes beeing run at least twice

// #define TOTALPROCESSES 3
// #define LOWEST 0  // TODO: Change as required
// #define MEDIUM 1  // TODO: Change as required
// #define HIGHEST 2 // TODO: Change as required

// int prio[TOTALPROCESSES] = {LOWEST, MEDIUM, HIGHEST};

// void testPrio() {
//   int pids[TOTALPROCESSES];
//   char *argv[] = {0};
//   int i;

//   for (i = 0; i < TOTALPROCESSES; i++)
//     pids[i] = myCreateProcess("endlessLoopPrint", 0, argv);

//   bussyWait(WAIT);
//   printf("\nCHANGING PRIORITIES...\n");

//   for (i = 0; i < TOTALPROCESSES; i++)
//     myNice(pids[i], prio[i]);

//   bussyWait(WAIT);
//   printf("\nBLOCKING...\n");

//   for (i = 0; i < TOTALPROCESSES; i++)
//     myBlock(pids[i]);

//   printf("CHANGING PRIORITIES WHILE BLOCKED...\n");

//   for (i = 0; i < TOTALPROCESSES; i++)
//     myNice(pids[i], MEDIUM);

//   printf("UNBLOCKING...\n");

//   for (i = 0; i < TOTALPROCESSES; i++)
//     myUnblock(pids[i]);

//   bussyWait(WAIT);
//   printf("\nKILLING...\n");

//   for (i = 0; i < TOTALPROCESSES; i++)
//     myKill(pids[i]);
// }
