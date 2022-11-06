// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "../include/schedluerEngine.h"

#include "../include/fileDescriptorManager.h"
#include "../include/interrupts.h"
#include "../include/list.h"
#include "../include/memoryManager.h"
#include "../include/naiveConsole.h"
static unsigned long nextProcessPid = 0;

static struct head *psReady[CANT_PRIORITIES];

static struct head *psWaiting[CANT_PRIORITIES];

static struct Node *currentProcess;

static struct head *psBlocked = NULL;

static Node *idleProcces = NULL;

static int prioritiesQuatums[] = {10, 7, 5, 3, 1};

static int actualPriority = 0;

static int contextOwner = -1;

static int currentQuantum = 0;

static int psReadyCount = 0;

extern void _hlt();

extern long loadContext(int argC,
                        char argv[MAX_ARGUMENT_LENGTH][MAX_ARGUMENT_LENGTH],
                        long rsp, void *funcPointer);
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
  idleProcces = alloc(sizeof(Node));
  idleProcces->data = createProcessPCB(-1, 0, 0, NULL, 0, NULL);
  idleProcces->data->stackPointer = loadContext(
      0, idleProcces->data->argV, idleProcces->data->stackPointer, idle);
}

void idle(int arc, char argv[MAX_ARGUMENT_LENGTH][MAX_ARGUMENT_LENGTH]) {
  while (1) {
    _hlt();
  }
}

long switchContext(long currentRSP) {
  if (contextOwner == -1 && psReadyCount == 0) {
    return idleProcces->data->stackPointer;
  }

  if (contextOwner == -1) {
    contextOwner = 0;
    nextProcess();
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
    nextProcess();
  } else if (currentProcess->data->state == KILL) {
    freeProcess(currentProcess);
    psReadyCount--;

    nextProcess();
  } else if (currentQuantum <= 0 && psReadyCount > 0) {
    if (currentProcess->data->pid != -1)
      push(psWaiting[currentProcess->data->priority], currentProcess);
    nextProcess();
  }
}
void closeFds() {
  for (size_t i = 0; i < 2; i++) {
    close(i);  // TODO TENER CUIDADO DE NO CERRARLE OTRO FD A OTRO
  }
}
void freeProcess(struct Node *toFree) {
  unblockChilds();
  closeFds();
  freePidQueue(toFree->data->waitingPidList);
  freeMemory((void *)toFree->data->stack);
  freeMemory(toFree->data);
  freeMemory(toFree);
}
void sendToBlockedList() { push(psBlocked, currentProcess); }

void nextProcess() {
  int nextPos = (actualPriority) % CANT_PRIORITIES;
  if (psReadyCount == 0) {
    currentProcess = idleProcces;
    currentQuantum = 0;
    return;
  }
  while (peek(psReady[nextPos]) == NULL) {
    if (nextPos == (CANT_PRIORITIES - 1)) {
      for (int i = 0; i < CANT_PRIORITIES; i++) {
        pushAll(psReady[i], psWaiting[i]);
        cleanAll(psWaiting[i]);
      }
    }
    nextPos = (nextPos + 1) % CANT_PRIORITIES;
  }
  currentProcess = pop(psReady[nextPos]);
  currentQuantum = currentProcess->data->quantum;
}

void unblockChilds() {
  while (!pidQueueEmpty(currentProcess->data->waitingPidList)) {
    unblockProcess(pidPull(currentProcess->data->waitingPidList));
  }
}
void exitProces() {
  currentProcess->data->state = KILL;
  timerTickInt();
}
int unblockProcess(int pid) {
  Node *toUnblock = deleteNode(psBlocked, pid);
  if (toUnblock == NULL) return -1;

  toUnblock->data->state = READY;
  psReadyCount++;
  push(psWaiting[toUnblock->data->priority], toUnblock);
  return 1;
}
int getProcesses() { return psReadyCount; }

int loadFirstContext(void *funcPointer, int argC,
                     char argv[MAX_ARGUMENT_LENGTH][MAX_ARGUMENT_LENGTH],
                     int type, char *name) {
  int newProcessPriority = 0;

  if (contextOwner != -1) newProcessPriority = DEFAULT_PRIORITY;
  int myPid = nextProcessPid++;
  Node *newNode = alloc(sizeof(struct Node));
  newNode->data =
      createProcessPCB(myPid, newProcessPriority, type, name, argC, argv);

  newNode->data->stackPointer =
      loadContext(newNode->data->argC, newNode->data->argV,
                  newNode->data->stackPointer, funcPointer);
  psReadyCount += 1;
  push(psWaiting[newProcessPriority], newNode);
  return myPid;
}

PCB *createProcessPCB(int pid, int newProcessPriority, int type, char *name,
                      int argC,
                      char argv[MAX_ARGUMENT_LENGTH][MAX_ARGUMENT_LENGTH]) {
  PCB *data = alloc(sizeof(PCB));
  data->stack = (long)alloc(MAX_STACK);
  data->stackBase = data->stack + MAX_STACK;
  data->stackPointer = data->stack + MAX_STACK;
  data->pid = pid;
  data->quantum = prioritiesQuatums[newProcessPriority];
  data->type = type;
  data->priority = newProcessPriority;
  data->state = READY;
  data->name = name;
  data->waitingPid = -1;
  data->argC = argC;
  data->waitingPidList = newPidQueue(10);
  if (contextOwner == -1) {
    data->fd[0] = STDOUT;
    data->fd[1] = STDIN;
  } else {
    for (int i = 0; i < MAX_FD_PROCESS; i++) {
      data->fd[i] = currentProcess->data->fd[i];
    }
  }
  if (argC > 0) {
    for (int i = 1; i < argC; i++) {
      myStrcpy(argv[i], data->argV[i]);
    }
  }
  return data;
}

void bockCurrentProcess(int pidToWait) {
  currentProcess->data->state = BLOCK;
  psReadyCount--;
  timerTickInt();
}
void addWaitingQueue(int pidToWait) {
  Node *toWaiting = searchAndDelete(pidToWait);
  if (toWaiting == NULL) return;
  pidPush(toWaiting->data->waitingPidList, currentProcess->data->pid);
  if (toWaiting->data->state == BLOCK)
    push(psBlocked, toWaiting);
  else
    push(psWaiting[toWaiting->data->priority], toWaiting);
  bockCurrentProcess(pidToWait);
}
void yield() {
  currentQuantum = 0;
  timerTickInt();
}

int blockProcess(int pid) {
  if (currentProcess->data->pid == pid) {
    bockCurrentProcess(-1);
  } else {
    Node *blockProcess = searchAndDelete(pid);
    if (blockProcess == NULL) return -1;
    if (blockProcess->data->state != BLOCK) psReadyCount--;
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
    if (killProcess == NULL) return -1;
    if (killProcess->data->state != BLOCK) psReadyCount--;
    freeProcess(killProcess);
  }
  return 1;
}

struct Node *searchAndDelete(int pid) {
  for (int i = 0; i < CANT_PRIORITIES; i++) {
    Node *returnPCB = deleteNode(psReady[i], pid);
    if (returnPCB != NULL && returnPCB->data->state != KILL) return returnPCB;
  }
  for (int i = 0; i < CANT_PRIORITIES; i++) {
    Node *returnPCB = deleteNode(psWaiting[i], pid);
    if (returnPCB != NULL && returnPCB->data->state != KILL) return returnPCB;
  }
  Node *returnPCB = deleteNode(psBlocked, pid);
  if (returnPCB != NULL && returnPCB->data->state != KILL) return returnPCB;
  return NULL;
}

void nice(int pid, int priority) {
  if (priority >= CANT_PRIORITIES || priority < 0) return;

  if (currentProcess->data->pid == pid) {
    currentProcess->data->priority = priority;
    currentProcess->data->quantum = prioritiesQuatums[priority];
    return;
  }
  Node *processNewPriority = searchAndDelete(pid);
  if (processNewPriority == NULL) return;
  processNewPriority->data->priority = priority;
  processNewPriority->data->quantum = prioritiesQuatums[priority];
  if (processNewPriority->data->state == BLOCK) {
    push(psBlocked, processNewPriority);
  } else
    push(psWaiting[priority], processNewPriority);
}
int getFD(int contextOwner) {
  if (currentProcess != NULL)
    return currentProcess->data->fileDescriptor;
  else
    return -1;
}
int currentPid() { return currentProcess->data->pid; }

int *currentProcessFds() { return currentProcess->data->fd; }
