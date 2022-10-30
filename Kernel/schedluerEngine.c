// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "include/schedluerEngine.h"
#include "include/list.h"
#include "include/naiveConsole.h"
#include <MemoryManager.h>
#include <interrupts.h>
#include <naiveConsole.h>
// Incluimos la funcion que proviene de assembler para loadear el primer
// contexto
extern long loadContext(int window, int argC, char **argV, long rsp,
                        void *funcPointer);
extern void _hlt();
extern void _sti();
extern void timerTickInt();

void psDump() {
  ncPrint("NAME    PID     STACKBASE    STACKPOINTER    TYPE    PRIORITY \n");
  for (int i = 0; i < CANT_PRIORITIES; i++) {
    dumpList(psReady[i]);
    dumpList(psWaiting[i]);
  }

  dumpList(psBlocked);
}

void initialiseContextSchedluerEngine() {
  for (int i = 0; i < CANT_PRIORITIES; i++) {
    psReady[i] = newList();
    psWaiting[i] = newList();
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
    currentProcess = pop(psReady[actualPriority]);
    currentQuantum = currentProcess->quantum;
    return currentProcess->stackPointer;
  }

  currentProcess->stackPointer = currentRSP;

  setActualPriority();
  currentQuantum--;
  return currentProcess->stackPointer;
}

void setActualPriority() {
  if (currentProcess->state == BLOCK) {
    sendToBlockedList();
    actualPriority = nextProcess();
  } else if (currentProcess->state == KILL) {
    freeProcess(currentProcess);
    processesRunning--;
    actualPriority = nextProcess();
  } else if (currentQuantum <= 0) {
    push(psWaiting[currentProcess->priority], currentProcess);
    actualPriority = nextProcess();
  }
}

void freeProcess(PCB *toFree) {
  /*for (int i = 0; i < toFree->argC; i++)
  {
      freeMemory(toFree->argV[i]);
  }
freeMemory(toFree->argV);*/
  freePidQueue(toFree->waitingPidList);
  freeMemory(toFree->stackBase);
  freeMemory(toFree);
}
void sendToBlockedList() { push(psBlocked, currentProcess); }

char nextProcess() {

  int nextPos = (actualPriority) % CANT_PRIORITIES;

  while (peek(psReady[nextPos]) == NULL) {
    nextPos = (nextPos + 1) % CANT_PRIORITIES;
    if (nextPos == (CANT_PRIORITIES - 1)) {
      for (int i = 0; i < CANT_PRIORITIES; i++) {
        pushAll(psReady[i], psWaiting[i]);
        cleanAll(psWaiting[i]);
      }
    }
  }
  currentProcess = pop(psReady[nextPos]);
  currentQuantum = currentProcess->quantum;
}
long exitProces() {
  currentProcess->state = KILL;
  unblockChilds();
  if (currentProcess->type == FOREGROUND) {
    PCB *toReload = getNodeWaiting(psBlocked, currentProcess->pid);
    if (toReload != NULL) {
      toReload->state = READY;
      toReload->quantum = prioritiesQuatums[toReload->pid];
      push(psReady[toReload->priority], toReload);
      deleteNode(psBlocked, toReload->pid);
    }
  }
  return switchContext(0);
}
void unblockChilds() {
  // while (!pidQueueEmpty(currentProcess->waitingPidList)) {
  //   unblockProcess(pidPull(currentProcess->waitingPidList));
  // }
}
int unblockProcess(int pid) {
  PCB *toUnblock = deleteNode(psBlocked, pid);
  if (toUnblock == NULL)
    return -1;
  toUnblock->state = READY;
  push(psWaiting[toUnblock->priority], toUnblock);
  return 1;
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
  // newProcess->argC = argC;
  // newProcess->argV = argv;
  newProcess->waitingPidList = newPidQueue(500);
  newProcess->stackPointer =
      loadContext(window, argC, argv, newProcess->stackPointer, funcPointer);

  push(psReady[newProcessPriority], newProcess);
  processesRunning += 1;
  if (type == FOREGROUND) {
    autoBlock(newProcess->pid);
  }
  return newProcess->pid;
}

void autoBlock(int pidToWait) {
  currentProcess->state = BLOCK;
  currentProcess->waitingPid = pidToWait;
  yield();
}
void addWaitingQueue(int pidToWait) {

  PCB *toWaiting = searchAndDelete(pidToWait);
  if (toWaiting == NULL)
    return;
  pidPush(toWaiting->waitingPidList, currentProcess->pid);
  if (toWaiting->state == BLOCK)
    push(psBlocked, toWaiting);
  else
    push(psWaiting[toWaiting->priority], toWaiting);
  autoBlock(pidToWait);
}
void yield() {
  currentQuantum = 0;
  timerTickInt();
}

int blockProcess(int pid) {
  if (currentProcess->pid == pid)
    autoBlock(-1);
  else {
    PCB *blockProcess = searchAndDelete(pid);
    if (blockProcess == NULL) {
      ncPrint("ERROR: \n");
      ncPrintDec(pid);
      psDump();
      return -1;
    }
    blockProcess->state = BLOCK;
    push(psBlocked, blockProcess);
  }
  return 1;
}
int killProcess(int pid) {
  if (currentProcess->pid == pid)
    exitProces();
  else {
    PCB *killProcess = searchAndDelete(pid);
    if (killProcess == NULL) {

      ncPrint("ERROR: \n");

      ncPrintDec(pid);
      psDump();
      return -1;
    }
    freeProcess(killProcess);
  }
  return 1;
}

PCB *searchAndDelete(int pid) {
  for (int i = 0; i < CANT_PRIORITIES; i++) {
    PCB *returnPCB = deleteNode(psReady[i], pid);
    if (returnPCB != NULL)
      return returnPCB;
  }
  for (int i = 0; i < CANT_PRIORITIES; i++) {
    PCB *returnPCB = deleteNode(psWaiting[i], pid);
    if (returnPCB != NULL)
      return returnPCB;
  }
  return deleteNode(psBlocked, pid);
}

void nice(int pid, int priority) {
  if (priority >= CANT_PRIORITIES || priority < 0)
    return;
  if (currentProcess->pid == pid) {
    currentProcess->priority = priority;
    currentProcess->quantum = prioritiesQuatums[priority];
    return;
  }
  PCB *processNewPriority = searchAndDelete(pid);
  if (processNewPriority == NULL)
    return;
  processNewPriority->priority = priority;
  processNewPriority->quantum = prioritiesQuatums[priority];
  if (processNewPriority->state == BLOCK) {
    push(psBlocked, processNewPriority);
  } else
    push(psWaiting[priority], processNewPriority);
  dumpList(psWaiting[priority]);
  dumpList(psBlocked);
}
int getFD(int contextOwner) {
  if (currentProcess != NULL)
    return currentProcess->fileDescriptor;
  else
    return -1;
}
int currentPid() { return currentProcess->pid; }
