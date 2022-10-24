

#include <stdio.h>
#include <string.h>

#include "include/benchmarksEngine.h"
#include "include/test_util.h"

#define MAX_BLOCKS 128

typedef struct memoryManagerRequest {
  void *address;
  uint32_t size;
  int value;
} MemoryManagerRequest;

void passed(char *testName, Window window) {
  putsf_(" DONE : ", GREEN, window);
  puts_(testName, window);
}

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
    for (int i = 0; i < rq; i++)
      if (mmRqs[i].address) {

        char *current = (char *)mmRqs[i].address;

        for (int idx = 0; idx < mmRqs[i].size; idx++, current++)
          if (*current != mmRqs[i].value) {
            putsf_("test_mm: ", RED, window);
            putInteger(*current, window);
            puts_(" != ", window);
            putInteger(i, window);
            putc_('\n', window);
            return;
          }
      }

    // Free
    for (i = 0; i < rq; i++)
      if (mmRqs[i].address)
        free(mmRqs[i].address);
  }

  passed("test_mm \n", window);
}

void benchmarkMemoryManager(Window window, int argc,
                            char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {

  testMM(window, 20, 100);
}
