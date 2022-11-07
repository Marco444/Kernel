// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/math.h"

int isPrime(int num) {
  int i;

  for (i = 2; i <= num - 1; i++)
    if (num % i == 0) return 0;

  return i == num;
}

int integerOverflowAddition(int a, int b) {
  return !(0 < (a + b) && (a + b) < MAX_INT - 1);
}

int longOverflowAddition(unsigned long long a, unsigned long long b) {
  return !(0 < (a + b) && (a + b) < MAX_UNSIGNED_LONG_LONG - 1);
}

int isLongerThan(const char *s, int size) {
  for (int i = 0; i < size; i++)
    if (s[i] == NULL_) return 0;

  return 1;
}

uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base) {
  char *p = buffer;
  char *p1, *p2;
  uint32_t digits = 0;

  // Calculate characters for each digit
  do {
    uint32_t remainder = value % base;
    *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
    digits++;
  } while (value /= base);

  // Terminate string in buffer.
  *p = 0;

  // Reverse string in buffer.
  p1 = buffer;
  p2 = p - 1;
  while (p1 < p2) {
    char tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
    p1++;
    p2--;
  }

  return digits;
}