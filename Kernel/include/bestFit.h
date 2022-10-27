#ifndef __MM_H__
#define __MM_H__

#include <stdint.h>
#include <stdlib.h>

#define TOTAL_HEAP_SIZE 0x8000000
#define HEAP_START 0x600000
// TODO: OJO DONDE EMPIEZA

void heapInit(char * startingMemory);
void *heapAlloc(size_t size);
void heapFree(void *address);
void heapDump(int window);

#endif
