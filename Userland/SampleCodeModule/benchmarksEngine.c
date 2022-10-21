
#include "include/benchmarksEngine.h"
#include "include/vga.h"

#define STRESS_ALLOCS 100 
#define ALLOCS_SIZE 3

#define STRESS_ALLOCS2 100
#define ARRAYS_ALLOCATED 100

void passed(char * testName, Window window) {
  putsf_(" DONE : ", GREEN, window);
  puts_(testName, window);
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
  //stressTest2(window);
}
