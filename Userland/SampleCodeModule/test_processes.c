#include "include/testManager.h"

#include "include/commandsEngine.h"

enum State { RUNNING, BLOCKED, KILLED };

extern int loadProcess(CommandPtr cmd, Window window, int argc,
                       char args[MAX_ARGUMENT_COUNT][MAX_ARGUMENT],
                       int backGround, char *name);

typedef struct P_rq {
  int32_t pid;
  enum State state;
} p_rq;

void testProcesses(Window window, int argc,
                   char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {

  puts_("ps passed", window);
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;
  uint64_t max_processes = 2;
  char *argvAux[] = {0};

  // if (argc != 1) {return -1};
  //
  // if ((max_processes = satoi(argv[0])) <= 0) return -1;

  p_rq p_rqs[max_processes];

  while (1) {

    // Create max_processes processes
    for (rq = 0; rq < max_processes; rq++) {
      p_rqs[rq].pid = loadProcess(endless_loop, 1, 0, argv, 1, "endless_loop");
      if (p_rqs[rq].pid == -1) {
        puts_("test_processes: ERROR creating process\n", 0);
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
              puts_("test_processes: ERROR killing process\n", 0);
              return;
            }
            p_rqs[rq].state = KILLED;
            alive--;
          }
          break;

        case 1:
          if (p_rqs[rq].state == RUNNING) {
            if (sysBlockProcess(p_rqs[rq].pid) == -1) {
              puts_("test_processes: ERROR blocking process\n", 0);
              return;
            }
            // puts_("BLOQUEEE   ",0);
            // putInteger(p_rqs[rq].pid,0);
            p_rqs[rq].state = BLOCKED;
          }
          break;
        }
      }

      // Randomly unblocks processes
      for (rq = 0; rq < max_processes; rq++)
        if (p_rqs[rq].state == BLOCKED && GetUniform(100) % 2) {
          if (sysUnblockProcess(p_rqs[rq].pid) == -1) {
            puts_("test_processes: ERROR unblocking process\n", 0);
            return;
          }
          // puts_("Desbloquee ",0);
          // putInteger(p_rqs[rq].pid,0);
          p_rqs[rq].state = RUNNING;
        }
    }
  }
}
