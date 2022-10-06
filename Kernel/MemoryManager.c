#include "./include/MemoryManager.h"
#include "./include/naiveConsole.h"

#include "./include/buddy.h"
#include "./include/bestFit.h"

#include <stdlib.h>

// #define BUDDY BUDDY  

typedef struct MemoryManagerCDT {
	Buddy manager;	
	void * startingMemory;
} MemoryManagerCDT;


void createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory) {

	memoryManager = (MemoryManagerADT) memoryForMemoryManager;	

	#ifdef BUDDY 
  	memoryManager->manager = buddy_new(MAX_MEMORY, managedMemory);
	#else 
		heap_init();
	#endif

	memoryManager->startingMemory = managedMemory;
}

void *allocMemory(const int memoryToAllocate) {

	void * addr = NULL;

	if(memoryToAllocate == 0) return addr;

	#ifdef BUDDY 
  	int offset = buddy_alloc(memoryManager->manager, memoryToAllocate);
		addr = (void *) (memoryManager->startingMemory + offset);
	#else 
		addr = heap_alloc(memoryToAllocate); 
	#endif

	return addr;
}

void freeMemory (void * const address) {

	if(address == NULL) return; 

	#ifdef BUDDY
		buddy_free(memoryManager->manager, (int) (address - memoryManager->startingMemory));
	#else
		heap_free(address);
		ncPrintAtFD("deleted from heap", 0);
	#endif
}

