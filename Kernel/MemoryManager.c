#include "./include/MemoryManager.h"
#include "./include/naiveConsole.h"

typedef struct MemoryManagerCDT {
	char * nextAddress;
} MemoryManagerCDT;

static MemoryManagerADT memoryManager;

void createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory) {
	memoryManager = (MemoryManagerADT) memoryForMemoryManager;
	memoryManager->nextAddress = managedMemory;

}

void *allocMemory( const int memoryToAllocate) {
	char *allocation = memoryManager->nextAddress;

	memoryManager->nextAddress += memoryToAllocate;

	return (void *) allocation;
}