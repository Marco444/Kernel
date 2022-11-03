// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
#include "include/MemoryManager.h"
#include "include/naiveConsole.h"
#include "include/pidQueue.h"
#include "include/schedluerEngine.h"
#include "include/semaphores.h"

/*
    Data type for a semaphore, which contains the value, name (if implemented)
    Turn type variable, and the amount of processes that use the semaphore
*/
typedef struct SemCDT {
  unsigned int id;
  int value;
  Turn semTurn;
  unsigned int processesCount;
  pidQueue processesWait;
} SemCDT;

/*
    Array that keeps track of all the semaphores created
*/
static Semaphore semaphores[MAX_SEM] = {NULL};
/*
    Counter of the semaphores running
*/
static unsigned int semaphoresCount = 0;
/*
    Check if Semaphore is valid?
*/
static int findSemaphore(Semaphore semaphore) {
  if (semaphore != NULL && semaphores[semaphore->id] != NULL) return SEM_OK;

  return SEM_NOT_EXISTS;
}

static void sleepProcess(Semaphore semaphore) {
  pidPush(semaphore->processesWait, currentPid());
  blockProcess(currentPid());
}

int semWait(Semaphore semaphore) {
  if (findSemaphore(semaphore) == SEM_NOT_EXISTS) return SEM_NOT_EXISTS;

  tryLock(&(semaphore->semTurn));

  (semaphore->value)--;

  if (semaphore->value < 0) {
    semaphore->value = 0;
    unlock(&(semaphore->semTurn));
    sleepProcess(semaphore);
    return SEM_OK;
  }
  unlock(&(semaphore->semTurn));
  return SEM_OK;
}

int semSignal(Semaphore semaphore) {
  if (semaphore == NULL || findSemaphore(semaphore) == SEM_NOT_EXISTS)
    return SEM_NOT_EXISTS;

  // In case that there's no process, increment the value
  if (pidQueueEmpty(semaphore->processesWait)) (semaphore->value)++;

  // Otherwise Wake up one process blocked by the wait.
  // do not increment semaphore, because this process will take the place
  if (semaphore->value == 0 && !pidQueueEmpty(semaphore->processesWait)) {
    unlock(&(semaphore->semTurn));
    unblockProcess(pidPull(semaphore->processesWait));
    return SEM_OK;
  }
  unlock(&(semaphore->semTurn));
  return SEM_OK;
}

static int semCreate(Semaphore *semaphore, int id) {
  // In case that we have reached the limit for semaphores
  if (semaphoresCount >= MAX_SEM - 1) return SEM_SIZE_LIMIT_REACHED;

  // Alloc for the semaphore
  *semaphore = alloc(sizeof(SemCDT));

  semaphores[id] = *semaphore;
  (*semaphore)->id = id;
  (*semaphore)->processesCount = 1;
  (*semaphore)->value = 1;
  (*semaphore)->semTurn = 0;
  (*semaphore)->processesWait = newPidQueue(MAX_PROCESSES);
  semaphoresCount++;
  return SEM_OK;
}

Semaphore semOpen(int id) {
  // We check that the id is valid
  if (id < 0 || id >= MAX_SEM) return NULL;

  // In case the semaphore already exists, we return the one that exists
  if (semaphores[id] != NULL) {
    // In case we have reached the limit of processes using this semaphore
    if (semaphores[id]->processesCount == MAX_PROCESSES) return NULL;
    // There's one more process using this semaphore
    (semaphores[id]->processesCount)++;
    // Return the pointer to the semaphore
    return semaphores[id];
  }

  // Otherwise we'll create it
  Semaphore toReturn;

  if (semCreate(&toReturn, id) != SEM_OK) return NULL;

  return toReturn;
}

int semClose(Semaphore semaphore) {
  // We check that the semahpore given is valid
  if (findSemaphore(semaphore) != SEM_OK) return SEM_NOT_EXISTS;

  // There's one less process using this semaphore
  (semaphore->processesCount)--;

  // In case the last process closed the semaphore, it will be destroid
  if (semaphore->processesCount == 0) {
    int id = semaphore->id;
    freeMemory(semaphore);
    semaphores[id] = NULL;
    semaphoresCount--;
  }

  return SEM_OK;
}

int getNextAvailableSemaphore() {
  for (int i = 0; i < MAX_SEM; i++) {
    if (semaphores[i] != NULL) return i;
  }
  return SEM_SIZE_LIMIT_REACHED;
}

int initializeSemaphoreSystem() {}

void semState() {
  ncPrint("ID       VALUE       PROCESSES\n");
  for (int i = 0; i < MAX_SEM; i++) {
    if (semaphores[i] != NULL) {
      ncPrintDec(semaphores[i]->id);
      ncPrint("         ");
      ncPrintDec(semaphores[i]->value);
      ncPrint("         ");
      ncPrintDec(semaphores[i]->processesCount);
      ncPrint("\n");
    }
  }
}
