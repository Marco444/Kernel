#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdlib.h>

#define MAX_MEMORY 131072

typedef struct MemoryManagerCDT *MemoryManagerADT;

void createMemoryManager(void *const managedMemory);

void *allocMemory(const int memoryToAllocate);

void freeMemory(void *const address);

#endif
