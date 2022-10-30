#include "include/pidQueue.h"
#include "include/MemoryManager.h"
#include <stddef.h>
#include <stdlib.h>

typedef struct pidQueueCDT {
  unsigned int *array;
  unsigned int size;
  unsigned int firstPos;
  unsigned int lastPos;
  short int empty;
} pidQueueCDT;

pidQueue newPidQueue(unsigned int size) {
  if (size <= 0)
    return NULL;

  pidQueue toReturn = allocMemory(sizeof(pidQueueCDT));

  toReturn->array = allocMemory(size * sizeof(int));
  toReturn->firstPos = 0;
  toReturn->lastPos = 0;
  toReturn->size = size;
  toReturn->empty = 1;

  return toReturn;
}

int pidPull(pidQueue queue) {
  if (queue == NULL || pidQueueEmpty(queue))
    return PID_QUEUE_ERROR;

  unsigned int toReturn = queue->array[queue->firstPos];
  queue->firstPos = (queue->firstPos + 1) % queue->size;

  if (queue->firstPos == queue->lastPos)
    queue->empty = 1;

  return toReturn;
}

int pidPush(pidQueue queue, int newValue) {
  if (pidQueueFull(queue))
    return PID_QUEUE_ERROR;

  queue->array[queue->lastPos] = newValue;

  queue->lastPos = (queue->lastPos + 1) % queue->size;

  queue->empty = 0;

  return PID_QUEUE_OK;
}

int pidQueueEmpty(pidQueue queue) { return queue->empty; }

int pidQueueFull(pidQueue queue) {
  return (!(queue->empty) && (queue->lastPos == queue->firstPos)) ? 1 : 0;
}

int pidQueueFull(pidQueue queue) {
  return (!(queue->empty) && (queue->lastPos == queue->firstPos));
}

void freePidQueue(pidQueue queue) {
  freeMemory(queue->array);
  freeMemory(queue);
}