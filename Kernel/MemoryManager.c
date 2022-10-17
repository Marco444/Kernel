#include "./include/MemoryManager.h"
#include "./include/naiveConsole.h"

#include "./include/buddy.h"
#include "./include/bestFit.h"

#include <stdlib.h>


typedef struct MemoryManagerCDT {
	Buddy manager;	
	void * startingMemory;
	char *nextAddress;
} MemoryManagerCDT;


static inline void * memoryFromOffset(int offset) {
	return (void *) (memoryManager->startingMemory + offset);
}

void createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory) {

	memoryManager = (MemoryManagerADT) memoryForMemoryManager;	

	#ifdef BUDDY 
  	memoryManager->manager = buddy_new(MAX_MEMORY, managedMemory);
	#elif HEAP 
		heap_init();
	#else
		memoryManager->nextAddress = managedMemory; 
	#endif

	memoryManager->startingMemory = managedMemory;
}

void *allocMemory(const int memoryToAllocate) {

	void * addr = NULL;

	if(memoryToAllocate == 0) return addr;

	#ifdef BUDDY 
  	addr = memoryFromOffset(buddy_alloc(memoryManager->manager, memoryToAllocate));
	#elif HEAP 
		addr = heap_alloc(memoryToAllocate); 
	#else
		addr = memoryManager->nextAddress;
		memoryManager->nextAddress += (size_t) memoryToAllocate;
	#endif

	return addr;
}

void freeMemory (void * const address) {

	if(address == NULL) return; 

	#ifdef BUDDY
		buddy_free(memoryManager->manager, (int) (address - memoryManager->startingMemory));
	#elif HEAP
		heap_free(address);
	#else
	
	#endif
}

