// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
#include <stddef.h>
#include <stdint.h>

#include "../include/_string.h"
#include "../include/commandsEngine.h"
#include "../include/constants.h"
#include "../include/stdio.h"
#include "../include/syscalls.h"
#include "../include/testUtil.h"

#define SEM_ID 1
#define TOTAL_PAIR_PROCESSES 5

int64_t global;  // shared memory

void slowInc(int64_t *p, int64_t inc) {
  int64_t aux = *p;
  myYield();  // This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

void myProcessInc(int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {
  uint64_t n;
  int8_t inc;
  int8_t use_sem;

  if (argc != 4) {
    puts_("Invalid arguments for Increment process\n");
    exit_();
  }

  if ((n = atoi_(argv[2])) <= 0) {
    puts_("Invalid n\n");
    exit_();
  }
  if ((use_sem = atoi_(argv[3])) < 0) {
    puts_("Invalid use_sem\n");
    exit_();
  }

  inc = 1;

  Semaphore sem;

  if (use_sem)
    if ((sem = semOpen(SEM_ID, 1)) == NULL) {
      puts_("test_sync: ERROR opening semaphore\n");
      exit_();
    }

  if (inc < 0)
    puts_("Inicia un proceso de decremento\n");
  else
    puts_("Inicia un proceso de incremento\n");

  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem) semWait(sem);

    slowInc(&global, inc);
    if (use_sem) semSignal(sem);
  }

  if (use_sem) semClose(sem);

  puts_("Termino 1 con valor: ");
  putInteger(global);
  puts_("\n");
  exit_();
  return;
}

void myProcessDec(int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {
  uint64_t n;
  int8_t inc;
  int8_t use_sem;

  if (argc != 4) {
    puts_("Invalid arguments for Increment process\n");
    exit_();
  }

  if ((n = atoi_(argv[2])) <= 0) {
    puts_("Invalid n\n");
    exit_();
  }
  if ((use_sem = atoi_(argv[3])) < 0) {
    puts_("Invalid use_sem\n");
    exit_();
  }

  inc = -1;

  Semaphore sem;

  if (use_sem)
    if ((sem = semOpen(SEM_ID, 1)) == NULL) {
      puts_("test_sync: ERROR opening semaphore\n");
      exit_();
    }

  if (inc < 0)
    puts_("Inicia un proceso de decremento\n");
  else
    puts_("Inicia un proceso de incremento\n");

  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem) semWait(sem);

    slowInc(&global, inc);
    if (use_sem) semSignal(sem);
  }

  if (use_sem) semClose(sem);

  puts_("Termino 1 con valor: ");
  putInteger(global);
  puts_("\n");
  exit_();
  return;
}

void testSync(int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

  if (argc != 4) exit_();

  global = 0;

  uint64_t i;
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    pids[i] = loadProcess(myProcessInc, 4, argv, 1, "my_process_dec");
    pids[i + TOTAL_PAIR_PROCESSES] =
        loadProcess(myProcessDec, 4, argv, 1, "my_process_inc");
  }

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    waitPid(pids[i]);
    waitPid(pids[i + TOTAL_PAIR_PROCESSES]);
  }
  puts_("Final value: ");
  putInteger(global);
  puts_("\n");

  putDefaultHeader();
  return;
}
