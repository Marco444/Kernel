#include "include/commandsEngine.h"
#include "include/constants.h"
#include "include/syscalls.h"
#include "include/testUtil.h"
#include <stdint.h>
#include "include/stdio.h"
#include <stddef.h>
#include "include/_string.h"

#define SEM_ID 1
#define TOTAL_PAIR_PROCESSES 1

int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc) {
  int64_t aux = *p;
  myYield(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
}
void myProcessInc(Window window, int argc,
                  char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {
  uint64_t n;
  int8_t inc;
  int8_t use_sem;
  
  if (argc != 3)
    return;

  // if ((n = atoi_(argv[0])) <= 0)
  //   return;
  // if ((inc = atoi_(argv[1])) == 0)
  //   return;
  // if ((use_sem = atoi_(argv[2])) < 0)
  //   return;

  n = 300;
  inc = -1;
  use_sem = 1;

  Semaphore sem;

  if (use_sem)
    if ((sem = semOpen(SEM_ID, 1)) == NULL) {
      puts_("test_sync: ERROR opening semaphore\n", 0);
      return;
    }
    

  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem)
      semWait(sem);
    
    slowInc(&global, inc);
    puts_("1 -> ", 0);
    putInteger(global, 0);
    puts_("\n", 0);
    if (use_sem)
      semSignal(sem);
  }

  if (use_sem)
    semClose(sem);

  puts_("Termino 1 con valor: ", 0);
  putInteger(global, 0);
  puts_("\n", 0);
  exit(0);

  return;
}

void myProcessInc2(Window window, int argc,
                  char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {
  uint64_t n;
  int8_t inc;
  int8_t use_sem;
  
  if (argc != 3)
    return;

  // if ((n = atoi_(argv[0])) <= 0)
  //   return;
  // if ((inc = atoi_(argv[1])) == 0)
  //   return;
  // if ((use_sem = atoi_(argv[2])) < 0)
  //   return;

  n = 300;
  inc = 1;
  use_sem = 1;

  Semaphore sem;

  if (use_sem)
    if ((sem = semOpen(SEM_ID, 1)) == NULL) {
      puts_("test_sync: ERROR opening semaphore\n", 0);
      return;
    }
    

  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem)
      semWait(sem);
    
    slowInc(&global, inc);
    puts_("2 -> ", 0);
    putInteger(global, 0);
    puts_("\n", 0);
    if (use_sem)
      semSignal(sem);
  }

  if (use_sem)
    semClose(sem);

  puts_("Termino 2 con valor: ", 0);
  putInteger(global, 0);
  puts_("\n", 0);
  exit(0);

  return;
}


uint64_t testSync(uint64_t argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {

  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

  if (argc != 2)
    return -1;

  // char *argvDec[] = {argv[0], "-1", argv[1], '\0'};
  // char *argvInc[] = {argv[0], "1", argv[1], '\0'};

  char *argvDec[] = {"3", "-1", "0", '\0'};
  char *argvInc[] = {"3", "1", "0", '\0'};

  global = 0;

  uint64_t i;
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    pids[i] = loadProcess(myProcessInc, 0, 3, argvDec, 1, "my_process_dec");
    pids[i + TOTAL_PAIR_PROCESSES] =
        loadProcess(myProcessInc2, 0, 3, argvInc, 1, "my_process_inc");
  }

 for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    waitPid(pids[i]);
    waitPid(pids[i + TOTAL_PAIR_PROCESSES]);
 }
  puts_("Final value: ", 0);
  putInteger(global, 0);
  puts_("\n", 0);

  return 0;
}
