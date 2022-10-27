#include "./include/MemoryManager.h"
#include "./include/bestFit.h"
#include "./include/buddy.h"
#include "./include/mmgr.h"
#include "./include/naiveConsole.h"
#include <stdlib.h>

//#define BUDDY BUDDY
//#define HEAP HEAP
#define HEAP2 HEAP2

//#define DUMMY DUMMY
typedef struct MemoryManagerCDT {
  Buddy manager;
  char *baseAddress;
} MemoryManagerCDT;

static inline void *memoryFromOffset(int offset) {
  if (offset < 0)
    return NULL;
  return (void *)(memoryManager->baseAddress + offset);
}

void createMemoryManager(void *const managedMemory) {

  memoryManager = (MemoryManagerADT)managedMemory;
#ifdef DUMMY
  memoryManager->baseAddress = managedMemory + sizeof(MemoryManagerCDT);
#endif
#ifdef BUDDY
  memoryManager->manager =
      buddy_new(MAX_MEMORY, managedMemory + sizeof(struct MemoryManagerCDT));
  memoryManager->baseAddress = (char *)0xFF0000;
#endif
#ifdef HEAP2
  initMgr();
#endif

#ifdef HEAP
  heapInit(managedMemory);
#endif
}

void *allocMemory(const int memoryToAllocate) {

  char *addr = NULL;

  if (memoryToAllocate == 0)
    return addr;
#ifdef DUMMY
  addr = memoryManager->baseAddress;

  memoryManager->baseAddress += memoryToAllocate;

#endif

#ifdef BUDDY
  addr =
      memoryFromOffset(buddy_alloc(memoryManager->manager, memoryToAllocate));
#endif

#ifdef HEAP
  addr = heapAlloc(memoryToAllocate);
#endif
#ifdef HEAP2
  return alloc(memoryToAllocate);
#endif
  return (void *)addr;
}

void freeMemory(void *const address) {

  if (address == NULL)
    return;
#ifdef DUMMY
  return;
#endif
#ifdef BUDDY
  buddy_free(memoryManager->manager,
             (int)((char *)address - memoryManager->baseAddress));
#endif

#ifdef HEAP
  heapFree(address);
#endif
#ifdef HEAP2
  return free(address);
#endif
}

void memoryDump(int window) {
#ifdef DUMMY
  return;
#endif
#ifdef BUDDY
  buddyDump(memoryManager->manager);
#endif

#ifdef HEAP
  heapDump(window);
#endif
#ifdef HEAP2
  return mem_dump();
#endif
}
