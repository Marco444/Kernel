// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
#include "../include/commandsEngine.h"
#include "../include/stdio.h"
#include "../include/testManager.h"

// #define CYCLES_TO_START 80000
enum State { RUNNING, BLOCKED, KILLED };

typedef struct P_rq {
  int pid;
  enum State state;
} p_rq;

void testProcesses(int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;
  uint64_t max_processes = 4;

  p_rq p_rqs[max_processes];
  while (1) {
    // Create max_processes processes
    for (rq = 0; rq < max_processes; rq++) {
      p_rqs[rq].pid = loadProcess(endless_loop, 0, argv, 1, "endless");
      if (p_rqs[rq].pid == -1) {
        puts_("testProcesses: ERROR creating process\n");
        return;
      } else {
        p_rqs[rq].state = RUNNING;
        alive++;
      }
    }

    // Randomly kills, blocks or unblocks processes until every one has been
    // killed
    while (alive > 0) {
      for (rq = 0; rq < max_processes; rq++) {
        action = GetUniform(100) % 2;

        switch (action) {
          case 0:
            if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED) {
              if (sysKillProcess(p_rqs[rq].pid) == -1) {
                puts_("testProcesses: ERROR killing process\n");
                return;
              }
              p_rqs[rq].state = KILLED;
              alive--;
            }
            break;

          case 1:
            if (p_rqs[rq].state == RUNNING) {
              if (sysBlockProcess(p_rqs[rq].pid) == -1) {
                putInteger(p_rqs[rq].pid);
                puts_("testProcesses: ERROR blocking process\n");
                return;
              }
              p_rqs[rq].state = BLOCKED;
            }
            break;
        }
      }

      // if(j++ == CYCLES_TO_START) putDefaultHeader();

      // Randomly unblocks processes
      for (rq = 0; rq < max_processes; rq++)
        if (p_rqs[rq].state == BLOCKED && GetUniform(100) % 2) {
          if (sysUnblockProcess(p_rqs[rq].pid) == -1) {
            puts_("testProcesses: ERROR unblocking process\n");
            return;
          }
          p_rqs[rq].state = RUNNING;
        }
    }
  }
}
