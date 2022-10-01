#include "./include/MemoryManager.h"
#include "./include/naiveConsole.h"
#include "./include/buddy.h"

typedef struct MemoryManagerCDT {
	Buddy buddyManager;	
	void * startingMemory;
} MemoryManagerCDT;

static MemoryManagerADT memoryManager;

void createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory) {
	
	//definimos al manager de manera hardcodeada porque no puede alocar memoria para si mismo
	memoryManager = (MemoryManagerADT) memoryForMemoryManager;	

	//inicializamos al buddy del memory manager, junto con la posicion desde donde almacena el buddy
	//junto con la memoria que administra.
	memoryManager->buddyManager = buddy_new(MAX_MEMORY, managedMemory);
	memoryManager->startingMemory = managedMemory;
}

void *allocMemory( const int memoryToAllocate) {
 
 	//le pido al buddy que aloquea una memoria
  int offset = buddy_alloc(memoryManager->buddyManager, memoryToAllocate);

	//devuelvo desde el comienzo de memoria del buddy mas el offset. TODO, puede
	//ser que no este haciendo bien bien el offset, verificar con GDB.
	return (void *) (memoryManager->startingMemory + offset);
}
