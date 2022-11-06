#ifndef PID_QUEUE_H
#define PID_QUEUE_H

#include "lib.h"

#define PID_QUEUE_ERROR -1
#define PID_QUEUE_OK 0

typedef struct pidQueueCDT *pidQueue;

pidQueue newPidQueue(unsigned int size);

int pidPull(pidQueue queue);

int pidPush(pidQueue queue, int newValue);

int pidQueueEmpty(pidQueue queue);

int pidQueueFull(pidQueue queue);

void freePidQueue(pidQueue queue);

#endif
