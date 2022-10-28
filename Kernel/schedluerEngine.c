// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "include/schedluerEngine.h"
#include "include/naiveConsole.h"
#include <MemoryManager.h>
#include <interrupts.h>
#include <naiveConsole.h>
// Incluimos la funcion que proviene de assembler para loadear el primer
// contexto
extern long loadContext(int window, char **argV, int argC, long rsp,
                        void *funcPointer);
extern void _hlt();
extern void _sti();
extern void timerTickInt();

void psDump() {
  ncPrint("NAME    PID     STACKBASE    STACKPOINTER    TYPE    PRIORITY \n");
  for (int i = 0; i < CANT_PRIORITIES; i++) {
    dumpList(psReady[i]);
  }
  dumpList(psBlocked);
}

void initialiseContextSchedluerEngine() {
  for (int i = 0; i < CANT_PRIORITIES; i++) {
    psReady[i] = newList();
  }
  psBlocked = newList();
}

long switchContext(long currentRSP) {
  // Aca debemos ver si el proceso actual esta bloqueado y si lo esta lo debemos
  // pasar a la cola de bloqueados.
  if (processesRunning == 0)
    return currentRSP;

  // TODO CAMBIAR
  if (contextOwner == -1) {
    contextOwner = 0;
    return current(psReady[actualPriority])->stackPointer;
  }

  current(psReady[actualPriority])->stackPointer = currentRSP;

  setActualPriority();

  current(psReady[actualPriority])->quantum--;
  return current(psReady[actualPriority])->stackPointer;
}

void setActualPriority() {
  if (current(psReady[actualPriority])->state == BLOCK) {
    sendToBlockedList();
    actualPriority = nextProcess();
  } else if (current(psReady[actualPriority])->state == KILL) {
    PCB *toDelete = deleteCurrentProcess(psReady[actualPriority]);
    freeProcess(toDelete);
    processesRunning--;
    actualPriority = nextProcess();
  } else if (current(psReady[actualPriority])->quantum <= 0) {
    current(psReady[actualPriority])->quantum =
        prioritiesQuatums[actualPriority];
    actualPriority = nextProcess();
  }
}

void freeProcess(PCB *toFree) {
  freeMemory(toFree->stackBase);
  freeMemory(toFree);
}
void sendToBlockedList() {

  PCB *toDelete = deleteCurrentProcess(psReady[actualPriority]);
  push(psBlocked, toDelete);
}

char nextProcess() {

  int nextPos = (actualPriority) % CANT_PRIORITIES;

  if (hasNext(psReady[nextPos]))
    next(psReady[nextPos]);
  while (!hasNext(psReady[nextPos])) {

    if (!hasNext(psReady[nextPos])) {
      initializeIterator(psReady[nextPos]);
      nextPos = (nextPos + 1) % CANT_PRIORITIES;
    } else
      next(psReady[nextPos]);
  }
  return nextPos;
}
long exitProces() {
  current(psReady[actualPriority])->state = KILL;
  if (current(psReady[actualPriority])->type == FOREGROUND) {
    PCB *toReload =
        getNodeWaiting(psBlocked, current(psReady[actualPriority])->pid);
    if (toReload) {
      toReload->state = READY;
      toReload->quantum = prioritiesQuatums[toReload->pid];
      push(psReady[toReload->priority], toReload);
      deleteNode(psBlocked, toReload->pid);
    }
  }
  return switchContext(0);
}

int getProcesses() { return processesRunning; }

int reloadProcess(int pid) {
  PCB *toReaload = deleteNode(psBlocked, pid);
  push(psReady[toReaload->priority], toReaload);
  processesRunning++;
  return processesRunning;
}

int loadFirstContext(void *funcPointer, int window, int argC, char **argv,
                     int type, char *name) {

  int newProcessPriority = 0;
  // Lo hago de esta manera para que la shell tenga una prioridad mayor
  if (processesRunning)
    newProcessPriority = DEFAULT_PRIORITY;

  PCB *newProcess = allocMemory(sizeof(PCB));
  newProcess->stackBase = allocMemory(MAX_STACK);
  newProcess->stackPointer = newProcess->stackBase + MAX_STACK;
  newProcess->pid = nextProcessPid++;
  newProcess->quantum = prioritiesQuatums[newProcessPriority];
  newProcess->type = type;
  newProcess->priority = newProcessPriority;
  newProcess->state = READY;
  newProcess->name = name;
  newProcess->stackPointer =
      loadContext(window, argC, argv, newProcess->stackPointer, funcPointer);

  push(psReady[newProcessPriority], newProcess);
  processesRunning += 1;
  if (type == FOREGROUND) {
    autoBlock(newProcess->pid);
  }
  return nextProcessPid - 1;
}

void autoBlock(int pidToWait) {
  current(psReady[actualPriority])->state = BLOCK;
  current(psReady[actualPriority])->waitingPid = pidToWait;
  timerTickInt();
}

void blockProcess(int pid) {
  if (current(psReady[actualPriority])->pid == pid)
    autoBlock(-1);
  else {
    int priorityOfProcess = searchNodeInAllPriorities(pid);
    if (priorityOfProcess == -1)
      return;
    PCB *blockProcess = deleteNode(psReady[priorityOfProcess], pid);
    blockProcess->state = BLOCK;
    push(psBlocked, blockProcess);
  }
}
void killProcess(int pid) {
  if (current(psReady[actualPriority])->pid == pid)
    exitProces();
  else {
    int priorityOfProcess = searchNodeInAllPriorities(pid);
    if (priorityOfProcess == -1)
      return;
    PCB *killProcess = deleteNode(psReady[priorityOfProcess], pid);
    freeProcess(killProcess);
  }
}
int searchNodeInAllPriorities(int pid) {
  for (int i = 0; i < CANT_PRIORITIES; i++) {
    PCB *returnNode = getNode(psReady[i], pid);
    if (returnNode != NULL)
      return i;
  }
  return -1;
}
int getFD(int contextOwner) {
  PCB *auxData = current(psReady[actualPriority]);
  if (auxData)
    return auxData->fileDescriptor;
  else
    return 0;
}
