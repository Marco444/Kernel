#ifndef __MM_H__
#define __MM_H__

#include <stdint.h>
#include <stdlib.h>

#define TOTAL_HEAP_SIZE 0x8000000
#define HEAP_START 0x600000
// TODO: OJO DONDE EMPIEZA

void heap_init();
void *heap_alloc(size_t size);
void heap_free(void *address);
void heapDump(int window);

#endif
