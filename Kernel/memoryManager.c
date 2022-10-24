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
  // if(offset < 0) return NULL;
  return (void *)(memoryManager->baseAddress + offset);
}

void createMemoryManager(void *const managedMemory) {

  memoryManager = (MemoryManagerADT)managedMemory;
  void *memoryToManage = managedMemory + sizeof(MemoryManagerCDT);

#ifdef BUDDY
  memoryManager->manager = buddy_new(MAX_MEMORY, memoryToManage);
  memoryManager->baseAddress = (char *)0xF00000;
#elif HEAP
  heap_init();
#else
  memoryManager->nextAddress = memoryToManage;
#endif
}

void *allocMemory(const int memoryToAllocate) {

  char *addr = NULL;

  if (memoryToAllocate == 0)
    return addr;

#ifdef BUDDY
  addr =
      memoryFromOffset(buddy_alloc(memoryManager->manager, memoryToAllocate));
#elif HEAP
  addr = heap_alloc(memoryToAllocate);
#else
  addr = memoryManager->nextAddress;
  memoryManager->nextAddress += (size_t)memoryToAllocate;
#endif

  return (void *)addr;
}

void freeMemory(void *const address) {

  if (address == NULL)
    return;

#ifdef BUDDY
    // buddy_free(memoryManager->manager,
    //            (int)(address - memoryManager->manager));
#elif HEAP
  heap_free(address);
#else

#endif
}
