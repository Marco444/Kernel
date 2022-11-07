#ifndef MANAGER_MEMORY
#define MANAGER_MEMORY

#include <stddef.h>
#include <stdint.h>

#include "../include/naiveConsole.h"

#define TOTAL_MEMORY 0x20000000
#define TOTAL_HEAP_SIZE 0x10000000

#define MINIMUM_ALLOCABLE_SIZE ((size_t)(blockLinkSize << 1))

#define BYTE_ALIGNMENT 0
#define BYTE_ALIGNMENT_MASK 0x0007

void memoryDump();

void createMemoryManager();

void *allocMemory(size_t size);
void freeMemory(void *address);

#endif
