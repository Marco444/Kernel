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
static unsigned long nextProcessPid = 0;
/*
 *Defino un array de los diferentes niveles de procesos
 *Por default la jerarquia del proceso va a ser 2
 */
static struct head *psReady[CANT_PRIORITIES];

static struct head *psWaiting[CANT_PRIORITIES];

static struct Node *currentProcess;
/*
 *Puntero a la lista en donde vamos a tener los procesos qe estan esperando
 *por su hijo
 */
static struct head *psBlocked = NULL;
/*
 *Defino un array statico el cual va a guardar los quatums que va a tener cada
 *nivel de privilegios
 */
static int prioritiesQuatums[] = {10, 7, 5, 3, 1};
/*
 *Variable que nos dice en que prioridad estamos corriendo
 *La seteo en 0 pues es la prioridad mas importante y es en la que vamos a
 *trabajar
 */
static int actualPriority = 0;
/*
 *Esta variable nos permite generar un pid para cada proceso
 */
/*
    TODO Deberia eliminar esto
*/
static int contextOwner = -1;

static int currentQuantum = 0;

/*
 * Defino la cantidad de procesos que tengo corriendo en este mismo momento
 * Obs: siempre tiene que ser menor que MAX_PROCESES
 */
static int processesRunning = 0;
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
    currentQuantum = currentProcess->data->quantum;
    return currentProcess->data->stackPointer;
  }

  currentProcess->data->stackPointer = currentRSP;

  setActualPriority();
  currentQuantum--;
  return currentProcess->data->stackPointer;
}

void setActualPriority() {
  if (currentProcess->data->state == BLOCK) {
    sendToBlockedList();
    actualPriority = nextProcess();
  } else if (currentProcess->data->state == KILL) {
    freeProcess(currentProcess);
    processesRunning--;
    actualPriority = nextProcess();
  } else if (currentQuantum <= 0) {
    push(psWaiting[currentProcess->data->priority], currentProcess);
    actualPriority = nextProcess();
  }
}

void freeProcess(struct Node *toFree) {
  /*for (int i = 0; i < toFree->argC; i++)
  {
      freeMemory(toFree->argV[i]);
  }
freeMemory(toFree->argV);*/
  // freePidQueue(toFree->data->waitingPidList);
  freeMemory(toFree->data->stackBase);
  freeMemory(toFree->data);
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
  currentQuantum = currentProcess->data->quantum;
}
long exitProces() {
  currentProcess->data->state = KILL;
  unblockChilds();
  if (currentProcess->data->type == FOREGROUND) {
    Node *toReload = getNodeWaiting(psBlocked, currentProcess->data->pid);
    if (toReload != NULL) {
      toReload->data->state = READY;
      toReload->data->quantum = prioritiesQuatums[toReload->data->pid];
      push(psReady[toReload->data->priority], toReload);
      deleteNode(psBlocked, toReload->data->pid);
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
  Node *toUnblock = deleteNode(psBlocked, pid);
  if (toUnblock == NULL)
    return -1;
  toUnblock->data->state = READY;
  push(psWaiting[toUnblock->data->priority], toUnblock);
  return 1;
}
int getProcesses() { return processesRunning; }

int reloadProcess(int pid) {
  Node *toReaload = deleteNode(psBlocked, pid);
  push(psReady[toReaload->data->priority], toReaload);
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
  if (newProcess == NULL)
    return -1;
  newProcess->stackBase = allocMemory(MAX_STACK);
  if (newProcess->stackBase == NULL)
    return -1;
  newProcess->stackPointer = newProcess->stackBase + MAX_STACK;
  newProcess->pid = nextProcessPid;
  newProcess->quantum = prioritiesQuatums[newProcessPriority];
  newProcess->type = type;
  newProcess->priority = newProcessPriority;
  newProcess->state = READY;
  newProcess->name = name;
  // newProcess->argC = argC;
  // newProcess->argV = argv;
  // newProcess->waitingPidList = newPidQueue(500);
  newProcess->fd[0] = getstdin();
  newProcess->fd[1] = getstdout();
  newProcess->stackPointer =
      loadContext(window, argC, argv, newProcess->stackPointer, funcPointer);
  Node *newNode = allocMemory(sizeof(struct Node));
  if (newNode == NULL)
    return -1;
  newNode->data = newProcess;
  push(psReady[newProcessPriority], newNode);
  processesRunning += 1;
  if (type == FOREGROUND) {
    autoBlock(newProcess->pid);
  }
  return nextProcessPid++;
}

void autoBlock(int pidToWait) {
  currentProcess->data->state = BLOCK;
  currentProcess->data->waitingPid = pidToWait;
  yield();
}
void addWaitingQueue(int pidToWait) {

  Node *toWaiting = searchAndDelete(pidToWait);
  if (toWaiting == NULL)
    return;
  pidPush(toWaiting->data->waitingPidList, currentProcess->data->pid);
  if (toWaiting->data->state == BLOCK)
    push(psBlocked, toWaiting);
  else
    push(psWaiting[toWaiting->data->priority], toWaiting);
  autoBlock(pidToWait);
}
void yield() {
  currentQuantum = 0;
  timerTickInt();
}

int blockProcess(int pid) {
  if (currentProcess->data->pid == pid)
    autoBlock(-1);
  else {
    Node *blockProcess = searchAndDelete(pid);
    if (blockProcess == NULL)
      return -1;

    blockProcess->data->state = BLOCK;
    push(psBlocked, blockProcess);
  }
  return 1;
}
int killProcess(int pid) {
  if (currentProcess->data->pid == pid)
    exitProces();
  else {
    Node *killProcess = searchAndDelete(pid);
    if (killProcess == NULL)
      return -1;

    freeProcess(killProcess);
  }
  return 1;
}

struct Node *searchAndDelete(int pid) {
  for (int i = 0; i < CANT_PRIORITIES; i++) {
    Node *returnPCB = deleteNode(psReady[i], pid);
    if (returnPCB != NULL)
      return returnPCB;
  }
  for (int i = 0; i < CANT_PRIORITIES; i++) {
    Node *returnPCB = deleteNode(psWaiting[i], pid);
    if (returnPCB != NULL)
      return returnPCB;
  }
  return deleteNode(psBlocked, pid);
}

void nice(int pid, int priority) {
  if (priority >= CANT_PRIORITIES || priority < 0)
    return;
  if (currentProcess->data->pid == pid) {
    currentProcess->data->priority = priority;
    currentProcess->data->quantum = prioritiesQuatums[priority];
    return;
  }
  Node *processNewPriority = searchAndDelete(pid);
  if (processNewPriority == NULL)
    return;
  processNewPriority->data->priority = priority;
  processNewPriority->data->quantum = prioritiesQuatums[priority];
  if (processNewPriority->data->state == BLOCK) {
    push(psBlocked, processNewPriority);
  } else
    push(psWaiting[priority], processNewPriority);
  dumpList(psWaiting[priority]);
  dumpList(psBlocked);
}
int getFD(int contextOwner) {
  if (currentProcess != NULL)
    return currentProcess->data->fileDescriptor;
  else
    return -1;
}
int currentPid() { return currentProcess->data->pid; }
