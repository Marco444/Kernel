#include "./include/MemoryManager.h"
#include "./include/naiveConsole.h"
#include "./include/buddy.h"
#include <stdlib.h>

#define BUDDY BUDDY  

typedef struct MemoryManagerCDT {
	Buddy manager;	
	void * startingMemory;
} MemoryManagerCDT;


void createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory) {

	//definimos al manager de manera hardcodeada porque no puede alocar memoria para si mismo
	memoryManager = (MemoryManagerADT) memoryForMemoryManager;	

	//inicializamos al buddy del memory manager, junto con la posicion desde donde almacena el buddy
	//junto con la memoria que administra.
	#ifdef BUDDY 
  	memoryManager->manager = buddy_new(MAX_MEMORY, managedMemory);
	#else 

	#endif

	memoryManager->startingMemory = managedMemory;
}

void *allocMemory(const int memoryToAllocate) {

	void * addr = NULL;

	if(memoryToAllocate == 0) return addr;

	#ifdef BUDDY 
 		//le pido al buddy que aloquea una memoria
  	int offset = buddy_alloc(memoryManager->manager, memoryToAllocate);

		//devuelvo desde el comienzo de memoria del buddy mas el offset. TODO, puede
		//ser que no este haciendo bien bien el offset, verificar con GDB.
		addr = (void *) (memoryManager->startingMemory + offset);
	#else 
	
	#endif

	return addr;
}

void freeMemory (void * const address) {

	if(address == NULL) return; 

	#ifdef BUDDY
		buddy_free(memoryManager->manager, (int) (address - memoryManager->startingMemory));
	#else
		
	#endif
}

