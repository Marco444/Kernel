// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/stdio.h"

#include "include/syscalls.h"
#include "include/vga.h"

#define MSG_ERROR_READING "Error reading from fd \n"
#define MSG_ERROR_WRITING "Error writing to fd \n"

void putDefaultHeader() {
  printHeader(
      "                                     GDH OS                     "
      "                ",
      CYAN_BACKGROUND | WHITE);
}
void perror(char *str) { printHeader(str, WHITE | LIGHT_RED_BACKGROUND); }

void read(char *str, int fd) {
  if (sysRead(fd, str)) perror(MSG_ERROR_READING);
}
void write(char *str, int fd) {
  if (sysWrite(fd, str, WHITE)) perror(MSG_ERROR_WRITING);
}

void puts_(char *string) { sysWrite(STDOUT, string, WHITE | BLACK_BACKGROUND); }

void putsf_(char *string, char format) { sysWrite(STDOUT, string, format); }

void putc_(char c) {
  char character[2] = {c, 0};
  if (sysWrite(STDOUT, character, WHITE | BLACK_BACKGROUND))
    perror(MSG_ERROR_WRITING);
}

void putcf_(char c, char format) {
  char character[2] = {c, 0};
  sysWrite(STDOUT, character, format);
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
