#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char *cpuVendor(char *result);

void itoa(char* string, int num, int digits);

void regsSnapshot(uint64_t * regs);

void getRegsSnapshot(uint64_t * buffer);

#endif