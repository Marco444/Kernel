// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
#include <stdint.h>

#include "./include/stdio.h"
#include "./include/syscalls.h"
#include "./include/testUtil.h"
#include "include/commandsEngine.h"

#define MINORWAIT \
  1000000  // TODO: Change this value to prevent a process from flooding the
// screen
#define WAIT \
  100000000  // TODO: Change this value to make the wait long enough to see
             // processes beeing run at least twice

#define TOTALPROCESSES 3
#define LOWEST 4   // TODO: Change as required
#define MEDIUM 2   // TODO: Change as required
#define HIGHEST 0  // TODO: Change as required

int prio[TOTALPROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void testPrio() {
  int pids[TOTALPROCESSES];
  char argv[20][20];
  int i;

  for (i = 0; i < TOTALPROCESSES; i++)
    pids[i] = loadProcess(endless_loop_print, 0, argv, 1, "endlessLoopPrint");

  bussy_wait(WAIT);
  puts_("\nCHANGING PRIORITIES...\n");

  for (i = 0; i < TOTALPROCESSES; i++) sysNiceProcess(pids[i], prio[i]);

  bussy_wait(WAIT);
  puts_("\nBLOCKING...\n");

  for (i = 0; i < TOTALPROCESSES; i++) sysBlockProcess(pids[i]);

  puts_("CHANGING PRIORITIES WHILE BLOCKED...\n");

  for (i = 0; i < TOTALPROCESSES; i++) sysNiceProcess(pids[i], MEDIUM);

  puts_("UNBLOCKING...\n");

  for (i = 0; i < TOTALPROCESSES; i++) sysUnblockProcess(pids[i]);

  bussy_wait(WAIT);
  puts_("\nKILLING...\n");

  for (i = 0; i < TOTALPROCESSES; i++) sysKillProcess(pids[i]);

  putDefaultHeader();
}
