#include "./include/MemoryManager.h"
#include "./include/naiveConsole.h"
#include "./include/buddy.h"
#include "./include/bestFit.h"

#include <stdlib.h>


typedef struct MemoryManagerCDT {
	char *nextAddress;
} MemoryManagerCDT;

void createMemoryManager(void *const restrict managedMemory) {
	memoryManager = (MemoryManagerCDT *) managedMemory;
	memoryManager->nextAddress = managedMemory + sizeof(MemoryManagerCDT);

}

void * allocMemory( const int memoryToAllocate) {
	char *allocation = memoryManager->nextAddress;

	memoryManager->nextAddress += memoryToAllocate;

	return (void *) allocation;
}
void  freeMemory (void * const address){
return;
}