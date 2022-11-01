#include "include/testManager.h"

#include "include/commandsEngine.h"

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
  char *argvAux[] = {0};

  // if (argc != 1) {return -1};
  //
  // if ((max_processes = satoi(argv[0])) <= 0) return -1;

  p_rq p_rqs[max_processes];
  while (1) {

    // Create max_processes processes
    for (rq = 0; rq < max_processes; rq++) {
      p_rqs[rq].pid = loadProcess(endless_loop, 0, argv, 1, "endless");
      if (p_rqs[rq].pid == -1) {
        puts_("test_processes: ERROR creating process\n");
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

              putInteger(p_rqs[rq].pid);
              newLine();
              puts_("test_processes: ERROR killing process\n");
              putInteger(p_rqs[0].pid);
              newLine();
              putInteger(p_rqs[1].pid);
              newLine();
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
              puts_("test_processes: ERROR blocking process\n");
              putInteger(p_rqs[0].pid);
              newLine();
              putInteger(p_rqs[1].pid);
              newLine();
              return;
            }
            p_rqs[rq].state = BLOCKED;
          }
          break;
        }
      }

      // Randomly unblocks processes
      for (rq = 0; rq < max_processes; rq++)
        if (p_rqs[rq].state == BLOCKED && GetUniform(100) % 2) {
          if (sysUnblockProcess(p_rqs[rq].pid) == -1) {
            puts_("test_processes: ERROR unblocking process\n");
            putInteger(p_rqs[0].pid);
            newLine();
            putInteger(p_rqs[1].pid);
            newLine();
            return;
          }
          p_rqs[rq].state = RUNNING;
        }
    }
  }
}
