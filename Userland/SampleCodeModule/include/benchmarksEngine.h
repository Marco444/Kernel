


#ifndef BENCHMARK_ENGINE 
#define BENCHMARK_ENGINE  

#include "stdlib.h"
#include "lib.h"

#include <stdio.h>
#include <stdint.h>

// uint32_t GetUint();
//
// uint32_t GetUniform(uint32_t max);
//
// void passed(char * testName, Window window);
//
// uint8_t memcheck(void *start, uint8_t value, uint32_t size);
//
// int64_t satoi(char* str);
//
// void bussy_wait(uint64_t n);
//
// void endless_loop();
//
// void endless_loop_print(uint64_t wait);
//
void stressTest(Window window);


void test_mm(uint64_t argc, char *argv[], Window window, int iterations);

void benchmarkMemoryManager(Window window);

#endif
