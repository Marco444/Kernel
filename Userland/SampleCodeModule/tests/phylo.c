#include "../include/phylo.h"

#include <stddef.h>

#include "../include/_string.h"
#include "../include/commandsEngine.h"
#include "../include/stdio.h"
#include "../include/syscalls.h"

typedef struct PhyloCDT {
  int status;
  int pos;
  int pid;
} PhyloCDT;

typedef struct PhyloCDT* Phylo;

static Phylo phylos[MAX_PHYLOS];
static Semaphore forks[MAX_FORKS];

static int phylosCount = 0;

static void listenInput();
static Phylo newPhilosopher(int pos);
static void newFork(int pos);
static void deletePhilosopher(Phylo philosopher);
static void deleteAllPhilosophers();
static void closeAllForks();
static void printTable();

void initializePhylos(int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {
  puts_("Starting Phylos ... \n");
  puts_(
      "Press:\n   -'a' to add a philosopher \n   -'r' to remove a philosopher "
      "\n   -'q' to quit ");
  puts_("The number of philosopher must be between 5 and 10\n");

  // Create the first 5 philosophers
  for (int i = 0; i < MAX_PHYLOS; i++) {
    if (i < MIN_PHYLOS) {
      phylos[i] = newPhilosopher(i);
    } else {
      phylos[i] = NULL;
      forks[i] = NULL;
    }
  }

  phylosCount = 5;

  printTable();

  listenInput();
}

static void fakeSleep() {
  for (int i = 0; i < 90000000; i++) {
  }
}

void philosopher(int argc, char argv[20][20]) {
  if (argc != 2) return;

  int id = atoi_(argv[1]);
  // The philosopher will eat, think and repeat
  while (1) {
    phylos[id]->status = THINKING;
    // Think
    myYield();

    int left = (id - 1) % phylosCount;
    int right = id;

    fakeSleep();

    phylos[id]->status = HUNGRY;

    semWait(forks[left]);
    semWait(forks[right]);

    myYield();
    phylos[id]->status = EATING;

    printTable();

    fakeSleep();

    semSignal(forks[left]);
    semSignal(forks[right]);

    phylos[id]->status = THINKING;

    printTable();
  }
}

static void listenInput() {
  char key;
  while (1) {
    read(&key, STDIN);
    switch (key) {
      case ADD:
        if (phylosCount == MAX_PHYLOS) {
          puts_(
              "There's no more room for philosophers. Just up to 10 are "
              "allowed\n");
        } else {
          phylos[phylosCount] = newPhilosopher(phylosCount);
          phylosCount++;
          printTable();
        }
        break;

      case REMOVE:
        if (phylosCount == MIN_PHYLOS) {
          puts_("Minimum 5 philosophers\n");
        } else {
          deletePhilosopher(phylos[phylosCount - 1]);
          phylosCount--;
          phylos[phylosCount] = NULL;
          printTable();
        }
        break;

      case QUIT:
        deleteAllPhilosophers();
        closeAllForks();
        exit_();
        break;

      default:
        break;
    }
  }
}

static Phylo newPhilosopher(int id) {
  char name[14] = "Philosopher ";
  name[12] = id + '0';
  name[13] = 0;

  char argv[2][20];
  argv[0][0] = '\0';
  argv[1][0] = id + '0';
  argv[1][1] = 0;

  int newpid = loadProcess(philosopher, 2, argv, 1, name);

  Phylo newPhylo = sysAlloc(sizeof(PhyloCDT));

  newPhylo->pid = newpid;
  newPhylo->status = THINKING;

  newFork(id);

  return newPhylo;
}

static void newFork(int pos) {
  if ((forks[pos] = semOpen(pos + 1, 1)) == NULL) {
    puts_("There has been an error while openning a semaphore (semOpen)\n");
    deleteAllPhilosophers();
    closeAllForks();
    exit_();
  }
}

static void deletePhilosopher(Phylo philosopher) {
  sysKillProcess(philosopher->pid);
  sysFree(philosopher);
}

static void deleteAllPhilosophers() {
  for (int i = 0; i < MAX_PHYLOS; i++) {
    if (phylos[i] != NULL) {
      sysKillProcess(phylos[i]->pid);
      sysFree(phylos[i]);
    }
  }
}

static void closeAllForks() {
  for (int i = 0; i < MAX_FORKS; i++) {
    if (forks[i] != NULL) semClose(forks[i]);
  }
}

static void printTable() {
  puts_("\n    ");
  int i = 0;
  while (i < MAX_PHYLOS && phylos[i] != NULL) {
    switch (phylos[i]->status) {
      case EATING:
        puts_(" E ");
        break;

      case HUNGRY:
        puts_(" X ");
        break;

      case THINKING:
        puts_(" . ");
        break;

      default:
        break;
    }
    i++;
  }
  puts_("\n");
}