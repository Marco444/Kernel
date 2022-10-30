#include "include/commandsEngine.h"
#include "include/constants.h"
#include "include/syscalls.h"
#include "include/testUtil.h"
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

#define SEM_ID 1
#define TOTAL_PAIR_PROCESSES 2

// int64_t global; // shared memory

// void slowInc(int64_t *p, int64_t inc) {
//   uint64_t aux = *p;
//   // myYield(); // This makes the race condition highly probable
//   aux += inc;
//   *p = aux;
// }
// void myProcessInc(Window window, int argc,
//                   char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {
//   uint64_t n;
//   int8_t inc;
//   int8_t use_sem;

//   if (argc != 3)
//     return;

//   if ((n = satoi(argv[0])) <= 0)
//     return;
//   if ((inc = satoi(argv[1])) == 0)
//     return;
//   if ((use_sem = satoi(argv[2])) < 0)
//     return;

//   Semaphore sem;

//   if (use_sem)
//     if ((sem = semOpen(SEM_ID, 1)) != NULL) {
//       printf("test_sync: ERROR opening semaphore\n");
//       return;
//     }

//   uint64_t i;
//   for (i = 0; i < n; i++) {
//     if (use_sem)
//       semWait(sem);
//     slowInc(&global, inc);
//     if (use_sem)
//       semSignal(sem);
//   }

//   if (use_sem)
//     semClose(sem);

//   return;
// }

uint64_t testSync(uint64_t argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {

//   uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

//   if (argc != 2)
//     return -1;

//   char *argvDec[] = {argv[0], "-1", argv[1], '\0'};
//   char *argvInc[] = {argv[0], "1", argv[1], '\0'};

//   global = 0;

//   uint64_t i;
//   for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
//     pids[i] = loadProcess(myProcessInc, 0, argc, argvDec, 1, "my_process_dec");
//     pids[i + TOTAL_PAIR_PROCESSES] =
//         loadProcess(myProcessInc, 0, argc, argvInc, 1, "my_process_inc");
//   }

//   for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
//     my_wait(pids[i]);
//     my_wait(pids[i + TOTAL_PAIR_PROCESSES]);
//   }

//   printf("Final value: %d\n", global);

//   return 0;
}
