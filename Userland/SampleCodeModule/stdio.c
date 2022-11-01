// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/stdio.h"

/* assembly functions definitions */
void sysRead(int fd, char *buffer);
void sysWriteHeaderFormat(char *string, int format);
void sysWrite(int fd, char *string);
void sysWriteFormat(int fd, char *string, int format);

void read(char *str, int fd) { sysRead(fd, str); }
void write(char *str, int fd) { sysWrite(fd, str); }

void puts_(char *string) { sysWrite(STDOUT, string); }

void putsf_(char *string, char format) {
  sysWriteFormat(STDOUT, string, format);
}

void putc_(char c) {
  char character[2] = {c, 0};
  sysWrite(STDOUT, character);
}

void putcf_(char c, char format) {
  char character[2] = {c, 0};
  sysWriteFormat(STDOUT, character, format);
}

void deleteChar() { putc_('\b'); }

void getKey(char *buffer) { sysRead(STDIN, buffer); }

void putInteger(int num) {
  if (num < 0) {
    puts_("-");
    num *= -1;
  }
  char numToStr[64];
  uintToBase(num, numToStr, 10);
  puts_(numToStr);
}

void putHex(int num) {
  char numToStr[11];
  uintToBase(num, numToStr, 16);
  puts_(numToStr);
}

void putBin(int num) {
  char numToStr[11];
  uintToBase(num, numToStr, 2);
  puts_(numToStr);
}
void printHeader(char *string, int format) {
  sysWriteHeaderFormat(string, format);
}

void newLine() { putc_('\n'); }
