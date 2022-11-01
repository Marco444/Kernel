#include "include/stdio.h"
#include "include/testManager.h"

#define MAX_BLOCKS 128

typedef struct memoryManagerRequest {
  void *address;
  uint32_t size;
  int value;
} MemoryManagerRequest;

void testMM(Window window, int iterations, int maxMemory) {

  MemoryManagerRequest mmRqs[MAX_BLOCKS];

  int i = 0;

  while (1) {
    int rq = 0;
    int total = 0;

    if (iterations != -1 && i++ > iterations)
      break;

    // Request as many blocks as we can
    while (rq < MAX_BLOCKS && total < maxMemory) {

      // defino el tamanio que le asigne y le aloco su memoria
      mmRqs[rq].size = GetUniform(maxMemory - total - 1) + 1;
      mmRqs[rq].address = alloc(mmRqs[rq].size);

      // si aloque memoria (no es null) entonces sumo al total,
      // sino continuo intentando con el siguiente.
      if (mmRqs[rq].address) {
        total += mmRqs[rq].size;
        rq++;
      }
    }

    // Set
    for (int i = 0; i < rq; i++) {
      if (mmRqs[i].address) {
        memset(mmRqs[i].address, i, mmRqs[i].size);
        mmRqs[i].value = i;
      }
    }

    // Check
    for (int i = 0; i < rq; i++) {
      if (mmRqs[i].address) {
        // putHex((long)mmRqs[i].address, 0);
        // puts_(" to ", 0);
        // putHex((long)mmRqs[i].address + mmRqs[i].size, 0);
        // newLine(0);
        if (!memcheck(mmRqs[i].address, i, mmRqs[i].size)) {
          puts_("test_mm ERROR\n");
          return;
        }
      }
    }

    // Free
    for (i = 0; i < rq; i++)
      if (mmRqs[i].address)
        free(mmRqs[i].address);
  }
}
