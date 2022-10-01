#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdlib.h>

#define MAX_MEMORY 128

typedef struct MemoryManagerCDT *MemoryManagerADT;

void createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory);

void *allocMemory( const int memoryToAllocate);

#endif
