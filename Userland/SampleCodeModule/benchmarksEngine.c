
#include <stdio.h>
#include <string.h>

#include "include/benchmarksEngine.h"
#include "include/test_util.h"

#define STRESS_ALLOCS 1000 
#define ALLOCS_SIZE 3

#define STRESS_ALLOCS2 100
#define ARRAYS_ALLOCATED 100

#define TEST_MM_ITERATIONS 100
#define MAX_MEMORY 100

#define MAX_BLOCKS 128

typedef struct MM_rq{
  void *address;
  uint32_t size;
}mm_rq;


void passed(char * testName, Window window) {
  putsf_(" DONE : ", GREEN, window);
  puts_(testName, window);
}

void test_mm(uint64_t argc, char *argv[], Window window, int iterations){

  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;
  uint64_t max_memory;


  max_memory = 100;
  int i = 0;

  while (1){
    rq = 0;
    total = 0;

    if(iterations != -1 && i++ > iterations) break;

    // Request as many blocks as we can
    while(rq < MAX_BLOCKS && total < max_memory){

      //defino el tamanio que le asigne y le aloco su memoria
      mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
      mm_rqs[rq].address = alloc(mm_rqs[rq].size);

      //si aloque memoria (no es null) entonces sumo al total,
      //sino continuo intentando con el siguiente.
      if(mm_rqs[rq].address){
        total += mm_rqs[rq].size;
        rq++;
      }
    }

    // Set
    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        memset(mm_rqs[i].address, i, mm_rqs[i].size);

    // Check
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        if(!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)) {
          putsf_("test_mm ERROR\n", RED, window);
          return;
        }

    // Free
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        free(mm_rqs[i].address);
  } 

  passed("test_mm \n", window);
}

void stressTest(Window window) {

  void * mem[STRESS_ALLOCS];

  for(int i = 0; i < STRESS_ALLOCS; i++)    mem[i] = alloc(ALLOCS_SIZE);
  for(int i = 0; i < STRESS_ALLOCS; i++)    free(mem[i]);

  passed("Stress test 1 \n", window);
}

void stressTest2(Window window) {

  void * matrix[STRESS_ALLOCS][ARRAYS_ALLOCATED];

  for (int i = 0; i < ARRAYS_ALLOCATED; i++)
    for(int j = 0; j < STRESS_ALLOCS2; j++)
        matrix[j][i] = alloc(ALLOCS_SIZE);   

  for (int i = 0; i < ARRAYS_ALLOCATED; i++)
    for(int j = 0; j < STRESS_ALLOCS2; j++)
        free(matrix[j][i]);


  passed("Stress test 2 \n", window);
}

void benchmarkMemoryManager(Window window) {
  stressTest(window);
  stressTest2(window);
}
