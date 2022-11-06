// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
#include "include/stdio.h"
#include "include/testManager.h"

#define MAX_BLOCKS 128
#define CYCLES_TO_START 12

typedef struct memoryManagerRequest {
  void *address;
  int size;
  int value;
} MemoryManagerRequest;

void testMM(int iterations, int maxMemory) {
  MemoryManagerRequest mmRqs[MAX_BLOCKS];

  int j = 0;

  while (1) {
    int rq = 0;
    int total = 0;

    j++;

    if (iterations != -1 && j > iterations) break;

    // Request as many blocks as we can
    while (rq < MAX_BLOCKS && total < maxMemory) {
      mmRqs[rq].size = GetUniform(maxMemory - total - 1) + 1;
      mmRqs[rq].address = alloc(mmRqs[rq].size);

      if (mmRqs[rq].address) {
        total += mmRqs[rq].size;
        rq++;
      }
    }

    // if(j == CYCLES_TO_START) {
    //   putDefaultHeader();
    // }

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
        if (!memcheck(mmRqs[i].address, i, mmRqs[i].size)) {
          puts_("test_mm ERROR\n");
          return;
        }
      }
    }

    // Free
    for (int i = 0; i < rq; i++)
      if (mmRqs[i].address) free(mmRqs[i].address);
  }
}
