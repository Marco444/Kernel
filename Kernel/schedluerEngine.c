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
  for (int i = 0; i < CANT_PRIORITIES; i++) {
    dumpList(prioritiesReady[i]);
  }
  dumpList(waitingProcess);
}

void initialiseContextSchedluerEngine() {
  for (int i = 0; i < CANT_PRIORITIES; i++) {
    prioritiesReady[i] = newList();
  }
  waitingProcess = newList();
}

int toSwitch() {
  // Aca implementamos la manera mas basica de switchear los procesos
  // en base a los timer ticks. El mismo numbero de timer ticks para
  // todos los procesos. En si aca se podria crear una tabla adicional
  // con los timer tick que cada comando deberia esperar para ser
  // switcheado
  ticks++;

  if (ticks == TICKS) {
    ticks = 0;
    return 1;
  }

  return 0;
}

long switchContext(long rsp) {
  // Aca debemos ver si el proceso actual esta bloqueado y si lo esta lo debemos
  // pasar a la cola de bloqueados.
  if (processesRunning == 0)
    return rsp;
  if (contextOwner == -1) {
    contextOwner = 0;
    return actual(prioritiesReady[actualPriority])->data->stackPointer;
  }

  actual(prioritiesReady[actualPriority])->data->stackPointer = rsp;
  if (actual(prioritiesReady[actualPriority])->data->state == BLOCK) {
    sendToWaitingList();
    actualPriority = nextProcess();
    // ncPrintBase(actual(prioritiesReady[actualPriority])->data->pid, 10);
  } else if (actual(prioritiesReady[actualPriority])->data->state == KILL) {
    Node *toDelete = deleteCurrent(prioritiesReady[actualPriority]);
    freeMemory(toDelete->data->stackBase);
    freeMemory(toDelete->data);
    freeMemory(toDelete);
    processesRunning--;
    actualPriority = nextProcess();
  } else if (!actual(prioritiesReady[actualPriority])->data->quantum) {
    actual(prioritiesReady[actualPriority])->data->quantum =
        prioritiesQuatums[actualPriority];
    actualPriority = nextProcess();
  }
  actual(prioritiesReady[actualPriority])->data->quantum--;
  return actual(prioritiesReady[actualPriority])->data->stackPointer;
}

void sendToWaitingList() {

  Node *toDelete = deleteCurrent(prioritiesReady[actualPriority]);
  push(waitingProcess, toDelete->data);
  freeMemory(toDelete);
}

char nextProcess() {

  int nextPos = (actualPriority) % CANT_PRIORITIES;
  if (hasNext(prioritiesReady[nextPos]))
    next(prioritiesReady[nextPos]);
  while (!hasNext(prioritiesReady[nextPos]) &&
         !actual(prioritiesReady[nextPos])) {
    if (!hasNext(prioritiesReady[nextPos])) {
      initializeIterator(prioritiesReady[nextPos]);
      nextPos = (nextPos + 1) % CANT_PRIORITIES;
    } else
      next(prioritiesReady[nextPos]);
  }
  return nextPos;
}

long exitProces() {

  actual(prioritiesReady[actualPriority])->data->state = KILL;
  if (!actual(prioritiesReady[actualPriority])->data->type) {
    Node *aux = getNodeWaiting(
        waitingProcess, actual(prioritiesReady[actualPriority])->data->pid);
    if (aux) {
      aux->data->state = READY;
      push(prioritiesReady[0], aux->data);
      freeMemory(deleteNode(waitingProcess, aux->data->pid));
    }
  }
  return switchContext(0);
}

int getProcesses() { return processesRunning; }

int reloadProcess(int pid) {
  Node *toReaload = getNode(waitingProcess, pid);
  deleteNode(waitingProcess, toReaload->data->pid);
  push(prioritiesReady[toReaload->data->priority], toReaload->data);
  return processesRunning;
}

int loadFirstContext(void *funcPointer, int window, int argC, char **argv,
                     int type) {

  int newProcessPriority = 0;
  // Lo hago de esta manera para que la shell tenga una prioridad mayor
  if (processesRunning)
    newProcessPriority = DEFAULT_PRIORITY;

  // addNewProcess(newProcessPriority);
  Process *newProcess = allocMemory(sizeof(Process));
  // Aca deberia hacer una alloc pero lo dejo para luego
  newProcess->stackBase = allocMemory(MAX_STACK);
  newProcess->stackPointer = newProcess->stackBase + MAX_STACK - 1;
  newProcess->flagRunning = 1;
  newProcess->flagPaused = 0;
  newProcess->pid = nextProcessPid++;
  newProcess->quantum = prioritiesQuatums[newProcessPriority];
  newProcess->type = type;
  newProcess->priority = newProcessPriority;
  newProcess->name[0] = 'p';
  newProcess->name[1] = newProcess->pid + '0';
  newProcess->stackPointer =
      loadContext(window, argC, argv, newProcess->stackPointer, funcPointer);
  push(prioritiesReady[newProcessPriority], newProcess);
  processesRunning += 1;
  if (!type) {
    autoBlock(newProcess->pid);
  }
  return nextProcessPid - 1;
}

void autoBlock(int pidToWait) {
  Node *shell = getNode(prioritiesReady[actualPriority], 0);
  shell->data->state = BLOCK;
  shell->data->waitingPid = pidToWait;
  timerTickInt();
}
void blockProcess(int pid) {
  if (actual(prioritiesReady[actualPriority])->data->pid == pid)
    autoBlock(-1);
  else {
    int priorityOfProcess = searchNodeInAllPriorities(pid);
    if (priorityOfProcess == -1)
      return;
    Node *blockProcess = deleteNode(prioritiesReady[priorityOfProcess], pid);
    blockProcess->data->state = BLOCK;
    push(waitingProcess, blockProcess->data);
    freeMemory(blockProcess);
  }
}
// Aca vamos a tener que hacer como si fuese una syscall Bloqueante me parece
void killProcess(int pid) {
  if (actual(prioritiesReady[actualPriority])->data->pid == pid)
    exitProces();
  else {
    int priorityOfProcess = searchNodeInAllPriorities(pid);
    if (priorityOfProcess == -1)
      return;
    Node *blockProcess = deleteNode(prioritiesReady[priorityOfProcess], pid);
    freeMemory(blockProcess->data->stackBase);
    freeMemory(blockProcess->data);
    freeMemory(blockProcess);
  }
}
int searchNodeInAllPriorities(int pid) {
  for (int i = 0; i < CANT_PRIORITIES; i++) {
    Node *returnNode = getNode(prioritiesReady[i], pid);
    if (returnNode != NULL)
      return i;
  }
  return -1;
}
int getFD(int contextOwner) {
  Process *auxData = actual(prioritiesReady[actualPriority])->data;
  if (auxData)
    return auxData->fileDescriptor;
  else
    return 0;
}
