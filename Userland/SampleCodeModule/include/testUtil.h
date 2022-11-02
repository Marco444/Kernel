
#include "constants.h"
#include "vga.h"

#include <stdint.h>

uint32_t GetUint();
uint32_t GetUniform(uint32_t max);
uint8_t memcheck(void *start, uint8_t value, uint32_t size);
int64_t satoi(char *str);
void bussy_wait(uint64_t n);
void endless_loop(int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]);
void endless_loop_print(int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]);
