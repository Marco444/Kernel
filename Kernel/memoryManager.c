#include "./include/MemoryManager.h"
#include "./include/bestFit.h"
#include "./include/buddy.h"
#include "./include/naiveConsole.h"

#include <stdlib.h>

#define BUDDY BUDDY
//#define HEAP HEAP

typedef struct MemoryManagerCDT {
  Buddy manager;
  char *nextAddress, *baseAddress;
} MemoryManagerCDT;

static inline void *memoryFromOffset(int offset) {
  if (offset < 0)
    return NULL;
  return (void *)(memoryManager->baseAddress + offset);
}

void createMemoryManager(void *const managedMemory) {

  memoryManager = (MemoryManagerADT)managedMemory;
  void *memoryToManage = managedMemory + sizeof(MemoryManagerCDT);

#ifdef BUDDY
  memoryManager->manager = buddy_new(MAX_MEMORY, memoryToManage);
  memoryManager->baseAddress = (char *)0xF00000;
#endif

#ifdef HEAP
  heap_init();
#endif
}

void *allocMemory(const int memoryToAllocate) {

  char *addr = NULL;

  if (memoryToAllocate == 0)
    return addr;

#ifdef BUDDY
  addr =
      memoryFromOffset(buddy_alloc(memoryManager->manager, memoryToAllocate));
#endif

#ifdef HEAP
  addr = heap_alloc(memoryToAllocate);
#endif

  return (void *)addr;
}

void freeMemory(void *const address) {

  if (address == NULL)
    return;

#ifdef BUDDY
  buddy_free(memoryManager->manager,
             (int)((char *)address - memoryManager->baseAddress));
#endif

#ifdef HEAP
  heap_free(address);
#endif
}

void memoryDump(int window) {

#ifdef BUDDY
  buddyDump(memoryManager->manager);
#endif

#ifdef HEAP
  heapDump(window);
#endif
}
