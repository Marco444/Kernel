// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>

#include "../include/MemoryManager.h"
#include "../include/naiveConsole.h"

void *memset(void *destination, int32_t c, uint64_t length) {
  uint8_t chr = (uint8_t)c;
  char *dst = (char *)destination;

  while (length--) dst[length] = chr;

  return destination;
}

void *memcpy(void *destination, const void *source, uint64_t length) {
  /*
   * memcpy does not support overlapping buffers, so always do it
   * forwards. (Don't change this without adjusting memmove.)
   *
   * For speedy copying, optimize the common case where both pointers
   * and the length are word-aligned, and copy word-at-a-time instead
   * of byte-at-a-time. Otherwise, copy by bytes.
   *
   * The alignment logic below should be portable. We rely on
   * the compiler to be reasonably intelligent about optimizing
   * the divides and modulos out. Fortunately, it is.
   */
  uint64_t i;

  if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
      (uint64_t)source % sizeof(uint32_t) == 0 &&
      length % sizeof(uint32_t) == 0) {
    uint32_t *d = (uint32_t *)destination;
    const uint32_t *s = (const uint32_t *)source;

    for (i = 0; i < length / sizeof(uint32_t); i++) d[i] = s[i];
  } else {
    uint8_t *d = (uint8_t *)destination;
    const uint8_t *s = (const uint8_t *)source;

    for (i = 0; i < length; i++) d[i] = s[i];
  }

  return destination;
}

void itoa(char *string, int num, int digits) {
  for (int i = digits; i != 0; i--, num /= 10) {
    string[i - 1] = (num % 10) + '0';
  }
}
void myStrcpy(char *from, char *to) {
  int i = 0;
  while (from[i] != 0) {
    to[i] = from[i];
    i++;
  }
  to[i] = 0;
  return;
}

int strlen_(char *str) {
  const char *s;

  for (s = str; *s; ++s)
    ;
  return (s - str);
}

void *alloc(int size) {
  void *addr = allocMemory(size);
  if (addr == NULL) {
    ncPrintHeader("                       HUBO UN ERROR EN EL ALLOC ",
                  WHITE | LIGHT_RED_BACKGROUND);
  }
  return addr;
}