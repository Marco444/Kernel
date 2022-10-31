#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

void memoryDump();

void createMemoryManager(void *const managedMemory);

void *allocMemory(const int memoryToAllocate);

void freeMemory(void *const address);

#endif
