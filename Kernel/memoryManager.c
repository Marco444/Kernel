#include "include/MemoryManager.h"
#include "include/mmgr.h"

void createMemoryManager(void *const managedMemory) { initMgr(); }

void *allocMemory(const int memoryToAllocate) {
  return alloc((size_t)memoryToAllocate);
}

void freeMemory(void *const address) { free(address); }

void memoryDump() { return; }
