#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdlib.h>

#define MAX_MEMORY 1024

typedef struct MemoryManagerCDT *MemoryManagerADT;

static MemoryManagerADT memoryManager;

void createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory);

void *allocMemory( const int memoryToAllocate);

void  freeMemory (void * const address);

#endif
