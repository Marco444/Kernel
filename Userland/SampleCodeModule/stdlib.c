

#include "include/stdlib.h"

void free(const void *address) { sysFree(address); }

void *alloc(int size) { return sysAlloc(size); }
