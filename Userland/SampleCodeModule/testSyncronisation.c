#include "include/testUtil.h"
#include "include/syscalls.h"
#include <stdint.h>
#include <stdio.h>

#define NULL 0l
#define SEM_ID 1
#define TOTAL_PAIR_PROCESSES 2

int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc) {
  uint64_t aux = *p;
  myYield(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

uint64_t myProcessInc(uint64_t argc, char *argv[]) {
  uint64_t n;
  int8_t inc;
  int8_t use_sem;

  if (argc != 3)
    return -1;

  if ((n = satoi(argv[0])) <= 0)
    return -1;
  if ((inc = satoi(argv[1])) == 0)
    return -1;
  if ((use_sem = satoi(argv[2])) < 0)
    return -1;

  if (use_sem)
    if (!semOpen(SEM_ID, 1)) {
      printf("test_sync: ERROR opening semaphore\n");
      return -1;
    }

  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem)
      semWait(SEM_ID);
    slowInc(&global, inc);
    if (use_sem)
      semSignal(SEM_ID);
  }

  if (use_sem)
    semClose(SEM_ID);

  return 0;
}

uint64_t testSync(uint64_t argc, char *argv[]) { //{n, use_sem, 0}
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

  if (argc != 2)
    return -1;

  char *argvDec[] = {argv[0], "-1", argv[1], '\0'};
  char *argvInc[] = {argv[0], "1", argv[1], '\0'};

  global = 0;

  uint64_t i;
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    pids[i] = my_create_process("my_process_inc", 3, argvDec);
    pids[i + TOTAL_PAIR_PROCESSES] =
        my_create_process("my_process_inc", 3, argvInc);
  }

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    my_wait(pids[i]);
    my_wait(pids[i + TOTAL_PAIR_PROCESSES]);
  }

  printf("Final value: %d\n", global);

  return 0;
}
